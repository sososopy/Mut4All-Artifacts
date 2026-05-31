//source file
#include "../include/extern_redefinition_within_function_scope_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("localVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        std::string varName = VD->getNameAsString();
        if (processedVars.find(varName) != processedVars.end())
            return;

        processedVars.insert(varName);

        std::string typeName = VD->getType().getAsString();
        std::string externDecl = "extern " + typeName + " " + varName + ";";
        SourceLocation insertLoc = VD->getEndLoc().getLocWithOffset(1);

        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut44*/" + externDecl);
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasLocalStorage()).bind("localVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}