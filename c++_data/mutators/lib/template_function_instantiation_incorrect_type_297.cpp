//source file
#include "../include/template_function_instantiation_incorrect_type_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        for (auto *spec : FT->specializations()) {
            if (spec->getTemplateSpecializationKind() == TSK_ExplicitSpecialization || 
                spec->getTemplateSpecializationKind() == TSK_ExplicitInstantiationDefinition) {
                auto callExpr = spec->getBody();
                if (callExpr) {
                    std::string replacementType = "std::string";
                    std::string originalCall = stringutils::rangetoStr(*(Result.SourceManager), callExpr->getSourceRange());
                    std::string mutatedCall = originalCall;

                    size_t pos = mutatedCall.find("<");
                    if (pos != std::string::npos) {
                        size_t endPos = mutatedCall.find(">", pos);
                        if (endPos != std::string::npos) {
                            mutatedCall.replace(pos + 1, endPos - pos - 1, replacementType);
                            templateFunctionCalls[spec] = mutatedCall;
                        }
                    }
                }
            }
        }
    }

    for (const auto &entry : templateFunctionCalls) {
        const FunctionDecl *spec = entry.first;
        std::string mutatedCall = entry.second;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(spec->getSourceRange()), mutatedCall);
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}