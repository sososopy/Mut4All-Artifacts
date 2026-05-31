//source file
#include "../include/static_member_function_declaration_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("anonStruct")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      if (RD->isAnonymousStructOrUnion()) {
        std::string uniqueFuncName = "unimplementedFunction_" + std::to_string(rand());
        std::string staticFuncDecl = "static void " + uniqueFuncName + "(int param);";
        SourceLocation insertLoc = RD->getEndLoc();
        Rewrite.InsertTextBefore(insertLoc, "\n    " + staticFuncDecl);
      }
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = recordDecl(hasName("")).bind("anonStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}