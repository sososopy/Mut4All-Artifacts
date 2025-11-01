//source file
#include "../include/Alias_Template_Instantiation_In_Class_Templates_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        CTD->getLocation()))
            return;

        for (auto *Spec : CTD->specializations()) {
            if (!Spec->isCompleteDefinition())
                continue;

            for (auto *D : Spec->decls()) {
                if (auto *Alias = llvm::dyn_cast<clang::TypeAliasTemplateDecl>(D)) {
                    std::string aliasName = Alias->getNameAsString();
                    std::string instantiation = "template<> " + aliasName + "<>::type;";
                    Rewrite.InsertTextAfter(Spec->getEndLoc(), "\n/*mut311*/" + instantiation);
                }
            }
        }
    }
}

void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(hasDescendant(typeAliasTemplateDecl())).bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}