//source file
#include "../include/misuse_structured_bindings_with_constexpr_556.h"

// ========================================================================================================
#define MUT556_OUTPUT 1

void MutatorFrontendAction_556::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structuredBinding")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (auto *DRE = dyn_cast<DeclRefExpr>(VD->getInit()->IgnoreImplicit())) {
            if (auto *RD = dyn_cast<CXXRecordDecl>(DRE->getDecl()->getType()->getAsCXXRecordDecl())) {
                if (RD->isCompleteDefinition()) {
                    std::string structName = RD->getNameAsString();
                    std::string newDecl = "constexpr " + structName + " myInstance{};\n";
                    newDecl += "constexpr auto [" + VD->getNameAsString() + ", ...rest] = myInstance; /*mut556*/\n";
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), newDecl);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_556::MutatorASTConsumer_556::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasInitializer(anything()), hasType(autoType())).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}