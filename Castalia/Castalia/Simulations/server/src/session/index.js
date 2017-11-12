const activeSession = {};

export default {
  markStatus(sessionId, status, message) {
    activeSession[sessionId] = {id: sessionId, status, message}
  },

  getSession(sessionId) {
    return activeSession[sessionId]
  }
}
