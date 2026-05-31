//source file
#include "../include/Base_Class_Member_Init_Reference_Undeclared_Member_335.h"

// ========================================================================================================
#define MUT335_OUTPUT 1

void MutatorFrontendAction_335::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructors")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        const CXXRecordDecl *DerivedClass = CD->getParent();
        if (!DerivedClass || !DerivedClass->hasDefinition() || DerivedClass->getNumBases() == 0)
            return;

        for (const auto *Init : CD->inits()) {
            if (Init->isMemberInitializer()) {
                const FieldDecl *Field = Init->getMember();
                if (Field && Field->getParent() != DerivedClass) {
                    const Expr *E = Init->getInit();
                    if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(E)) {
                        std::string new_name = "non_existent_member";
                        Rewrite.ReplaceText(DRE->getSourceRange(), new_name);
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_335::MutatorASTConsumer_335::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl().bind("constructors");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}