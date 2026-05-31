//source file
#include "../include/introduce_variable_shadowing_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        
        if (!VD->hasLocalStorage() || VD->isStaticLocal())
            return;

        auto varName = VD->getNameAsString();
        auto varType = VD->getType().getAsString();
        
        SourceLocation insertLoc = VD->getEndLoc().getLocWithOffset(1);
        std::string shadowDecl = "\n/*mut34*/ " + varType + " " + varName + " = " + varName + ";";
        
        Rewrite.InsertTextAfterToken(insertLoc, shadowDecl);
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasLocalStorage()).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}