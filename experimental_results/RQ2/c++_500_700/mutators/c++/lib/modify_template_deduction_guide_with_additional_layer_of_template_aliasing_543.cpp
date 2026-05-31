//source file
#include "../include/modify_template_deduction_guide_with_additional_layer_of_template_aliasing_543.h"

// ========================================================================================================
#define MUT543_OUTPUT 1

void MutatorFrontendAction_543::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Alias = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("AliasTemplate")) {
        //Filter nodes in header files
        if (!Alias || !Result.Context->getSourceManager().isWrittenInMainFile(
                Alias->getLocation()))
            return;

        OriginalAlias = Alias;

        //Get the source code text of target node
        auto aliasName = Alias->getNameAsString();
        auto newAliasName = "New" + aliasName;

        //Perform mutation on the source code text by applying string replacement
        std::string newAliasDecl = "template <class T, class... Ts>\nusing " + newAliasName + " = " + aliasName + "<T, Ts...>;\n";
        Rewrite.InsertTextAfter(Alias->getEndLoc(), "\n/*mut543*/" + newAliasDecl);

        // Replace usages of the original alias with the new alias
        for (const auto &Use : Result.Context->getTranslationUnitDecl()->decls()) {
            if (const auto *NamedDecl = llvm::dyn_cast<clang::NamedDecl>(Use)) {
                if (NamedDecl->getNameAsString() == aliasName) {
                    if (const auto *TypeAlias = llvm::dyn_cast<clang::TypeAliasDecl>(Use)) {
                        auto aliasRange = TypeAlias->getSourceRange();
                        Rewrite.ReplaceText(aliasRange, newAliasName);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_543::MutatorASTConsumer_543::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}