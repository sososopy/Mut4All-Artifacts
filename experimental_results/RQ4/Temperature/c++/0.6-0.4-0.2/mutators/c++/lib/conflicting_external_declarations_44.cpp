//source file
#include "../include/conflicting_external_declarations_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("variableDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (VD->isLocalVarDeclOrParm() && !VD->isExternC()) {
            std::string varType = VD->getType().getAsString();
            std::string varName = VD->getNameAsString();
            std::string externDecl = "extern " + varType + " " + varName + ";";
            SourceLocation insertLoc = VD->getBeginLoc().getLocWithOffset(-1);
            Rewrite.InsertTextBefore(insertLoc, externDecl + "\n/*mut44*/");
        }
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(isExpansionInMainFile()).bind("variableDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}