//source file
#include "../include/Add_Template_Parameter_To_Specialization_441.h"

// ========================================================================================================
#define MUT441_OUTPUT 1

void MutatorFrontendAction_441::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("TemplateSpec")) {
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
        size_t insertPos = originalCode.find(">");

        if (insertPos != std::string::npos) {
            std::string newParam = ", typename TExtra";
            originalCode.insert(insertPos, newParam);

            size_t requiresPos = originalCode.find("requires");
            if (requiresPos != std::string::npos) {
                std::string newConstraint = " && std::is_integral_v<TExtra>";
                size_t endPos = originalCode.find("{", requiresPos);
                if (endPos != std::string::npos) {
                    originalCode.insert(endPos, newConstraint);
                }
            }

            size_t funPos = originalCode.find("auto fun");
            if (funPos != std::string::npos) {
                size_t paramPos = originalCode.find("(", funPos);
                if (paramPos != std::string::npos) {
                    std::string newParamInFun = "TExtra extra_param";
                    originalCode.insert(paramPos + 1, newParamInFun);
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), originalCode);
        }
    }
}
  
void MutatorFrontendAction_441::MutatorASTConsumer_441::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}