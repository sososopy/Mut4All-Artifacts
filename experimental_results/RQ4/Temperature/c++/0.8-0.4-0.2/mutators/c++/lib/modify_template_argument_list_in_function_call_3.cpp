//source file
#include "../include/modify_template_argument_list_in_function_call_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getExprLoc()))
            return;

        if (auto *TemplateSpecialization = dyn_cast<clang::FunctionDecl>(CallExpr->getCalleeDecl())) {
            if (TemplateSpecialization->getTemplateSpecializationArgs()) {
                llvm::ArrayRef<clang::TemplateArgument> TemplateArgs = TemplateSpecialization->getTemplateSpecializationArgs()->asArray();
                std::string modifiedArgs;

                for (unsigned i = 0; i < TemplateArgs.size(); ++i) {
                    if (i == 1) {
                        modifiedArgs += "std::string";
                    } else {
                        modifiedArgs += TemplateArgs[i].getAsType().getAsString();
                    }
                    if (i < TemplateArgs.size() - 1) {
                        modifiedArgs += ", ";
                    }
                }

                std::string originalText = clang::Lexer::getSourceText(
                    clang::CharSourceRange::getTokenRange(CallExpr->getSourceRange()),
                    Result.Context->getSourceManager(), Result.Context->getLangOpts()).str();

                size_t angleBracketPos = originalText.find('<');
                size_t closingBracketPos = originalText.find('>');

                if (angleBracketPos != std::string::npos && closingBracketPos != std::string::npos) {
                    std::string mutatedText = originalText.substr(0, angleBracketPos + 1) +
                                              modifiedArgs +
                                              originalText.substr(closingBracketPos);
                    Rewrite.ReplaceText(CallExpr->getSourceRange(), mutatedText);
                }
            }
        }
    }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("TemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}