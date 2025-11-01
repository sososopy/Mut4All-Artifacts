//source file
#include "../include/template_specialization_with_default_constraints_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        for (auto *Spec : TD->specializations()) {
            if (Spec->isCompleteDefinition()) {
                auto *Params = TD->getTemplateParameters();
                for (auto *Param : *Params) {
                    if (auto *TypeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                        if (TypeParam->hasTypeConstraint() && !TypeParam->hasDefaultArgument()) {
                            std::string DefaultType = "int"; // Example default type
                            templateDefaults[TD] = DefaultType;
                            std::string TemplateName = TD->getNameAsString();
                            std::string Mutation = "template <typename T = " + DefaultType + "> struct " + TemplateName + ";";
                            Rewrite.InsertTextBefore(TD->getBeginLoc(), "/*mut50*/" + Mutation + "\n");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl())).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}