//source file
#include "../include/Corrupt_Inheritance_Insert_Class_Keyword_270.h"

// ========================================================================================================
#define MUT270_OUTPUT 1

void MutatorFrontendAction_270::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecord")) {
        // Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;

        for (const auto &base : RD->bases()) {
            clang::SourceRange typeRange = base.getType().getSourceRange();
            if (typeRange.isValid()) {
                // Insert 'class ' before the base class name
                Rewrite.InsertText(typeRange.getBegin(), "class ", true, true);
            }
        }
    }
}
  
void MutatorFrontendAction_270::MutatorASTConsumer_270::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(anything())).bind("CXXRecord");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}