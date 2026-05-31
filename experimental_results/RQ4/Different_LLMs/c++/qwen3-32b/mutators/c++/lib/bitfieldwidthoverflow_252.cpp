//source file
#include "../include/BitfieldWidthOverflow_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("bitField")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->getName().empty())
            return;
        QualType T = FD->getType();
        std::string typeName = T.getAsString();
        std::string newExpr = "(" + typeName + ")-1 + 1";
        if (clang::Expr *bitWidth = FD->getBitWidth()) {
            SourceLocation start = bitWidth->getBeginLoc();
            SourceLocation end = bitWidth->getEndLoc();
            Rewrite.ReplaceText(SourceRange(start, end), newExpr);
        }
    }
}
  
void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(
        hasName(""),
        hasAncestor(cxxRecordDecl())
    ).bind("bitField");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}