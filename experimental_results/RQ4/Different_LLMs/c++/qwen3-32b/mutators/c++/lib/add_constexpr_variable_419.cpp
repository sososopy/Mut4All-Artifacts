//source file
#include "../include/Add_Constexpr_Variable_419.h"

// ========================================================================================================
#define MUT419_OUTPUT 1

void MutatorFrontendAction_419::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        if (VD->isConstexpr())
            return;
        if (!VD->hasICEInitializer(*Result.Context))
            return;
        if (!VD->hasConstantInitialization())
            return;
        if (TypeSourceInfo *TSI = VD->getTypeSourceInfo()) {
            SourceLocation typeLoc = TSI->getTypeLoc().getBeginLoc();
            if (typeLoc.isValid()) {
                Rewrite.InsertText(typeLoc, "constexpr ", false);
            }
        }
    }
}

void MutatorFrontendAction_419::MutatorASTConsumer_419::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(unless(isConstexpr()), hasInitializer(expr())).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}