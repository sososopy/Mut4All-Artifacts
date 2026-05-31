//source file
#include "../include/modify_template_parameter_pack_usage_533.h"

// ========================================================================================================
#define MUT533_OUTPUT 1

void MutatorFrontendAction_533::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateStruct")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *TS = dyn_cast<ClassTemplateSpecializationDecl>(TD->getTemplatedDecl())) {
            for (auto *FD : TS->decls()) {
                if (auto *FDecl = dyn_cast<FunctionDecl>(FD)) {
                    if (FDecl->getBody()) {
                        auto declStr = stringutils::rangetoStr(*(Result.SourceManager), FDecl->getSourceRange());
                        auto pos = declStr.find("requires(void(sizeof...(Ts)), false)");
                        if (pos != std::string::npos) {
                            declStr.erase(pos, std::string("requires(void(sizeof...(Ts)), false)").length());
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FDecl->getSourceRange()), declStr);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_533::MutatorASTConsumer_533::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl())).bind("TemplateStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}