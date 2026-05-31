//source file
#include "../include/Mutator_527.h"

// ========================================================================================================
#define MUT527_OUTPUT 1

void MutatorFrontendAction_527::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionMember")) {
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;

      if (UD->getInClassInitializer() == nullptr) {
        SourceLocation loc = UD->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, " = /*mut527*/;");
      }
    }
}
  
void MutatorFrontendAction_527::MutatorASTConsumer_527::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasParent(recordDecl(isUnion()))).bind("UnionMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}