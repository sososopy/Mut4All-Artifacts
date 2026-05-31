//source file
#include "../include/Transform_Variadic_Template_Lambda_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            auto *TPL = FD->getDescribedFunctionTemplate();
            if (!TPL)
                return;
            
            std::string funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            size_t lambdaPos = funcText.find("[]()");
            if (lambdaPos != std::string::npos) {
                std::string lambdaInsert = "__attribute__((diagnose_if(vals, \"message\", \"error\")))";
                funcText.insert(lambdaPos + 4, lambdaInsert);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), funcText);
            }
        }
    }
}
  
void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasName("f")).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}