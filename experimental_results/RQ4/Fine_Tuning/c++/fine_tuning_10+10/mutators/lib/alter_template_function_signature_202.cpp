//source file
#include "../include/Alter_Template_Function_Signature_202.h"

// ========================================================================================================
#define MUT202_OUTPUT 1

void MutatorFrontendAction_202::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateParams = TD->getTemplateParameters();
        if (templateParams->size() > 0) {
            auto param = templateParams->getParam(0);
            if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                std::string newTypeName = "NonExistentType";
                std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                size_t pos = originalCode.find(typeParam->getNameAsString());
                if (pos != std::string::npos) {
                    originalCode.replace(pos, typeParam->getNameAsString().length(), newTypeName);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), originalCode);
                }
            }
        }
    }
}

void MutatorFrontendAction_202::MutatorASTConsumer_202::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasAncestor(recordDecl())).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}