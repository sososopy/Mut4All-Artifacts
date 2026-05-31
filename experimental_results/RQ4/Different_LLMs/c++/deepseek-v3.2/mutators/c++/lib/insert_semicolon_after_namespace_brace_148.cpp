//source file
#include "../include/Mutator_Insert_Semicolon_After_Namespace_Brace_148.h"

// ========================================================================================================
#define MUT148_OUTPUT 1

void MutatorFrontendAction_148::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NamespaceDecl")) {
      //Filter nodes in header files
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;
      
      // Get the opening brace location
      SourceLocation BraceLoc = NS->getBeginLoc();
      if (BraceLoc.isInvalid()) return;
      
      // Find the opening brace token
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      Token Tok;
      if (Lexer::getRawToken(BraceLoc, Tok, SM, LangOpts, true)) return;
      
      // Check if this is a namespace with body
      if (!NS->hasBody()) return;
      
      // Get the body and its start location
      if (const CompoundStmt *Body = dyn_cast_or_null<CompoundStmt>(NS->getBody())) {
        SourceLocation BodyStart = Body->getBeginLoc();
        if (BodyStart.isInvalid()) return;
        
        // Insert semicolon after the opening brace
        Rewrite.InsertTextAfterToken(BodyStart, ";");
      }
    }
}
  
void MutatorFrontendAction_148::MutatorASTConsumer_148::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namespaceDecl().bind("NamespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}