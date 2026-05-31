//source file
#include "../include/Convert_Namespace_To_Invalid_Alias_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ND = Result.Nodes.getNodeAs<clang::NamespaceDecl>("namespaceDecl")) {
        if (!ND || !Result.Context->getSourceManager().isWrittenInMainFile(ND->getLocation()))
            return;
        std::string newName = "namespace " + ND->getNameAsString() + ";";
        SourceLocation loc = ND->getSourceRange().getBegin();
        SourceLocation endLoc = ND->getSourceRange().getEnd();
        Rewrite.ReplaceText(SourceRange(loc, endLoc), newName);
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl().bind("namespaceDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}