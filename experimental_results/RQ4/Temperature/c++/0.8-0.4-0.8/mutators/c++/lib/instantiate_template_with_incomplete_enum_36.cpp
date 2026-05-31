//source file
#include "../include/instantiate_template_with_incomplete_enum_36.h"

// ========================================================================================================
#define MUT36_OUTPUT 1

void MutatorFrontendAction_36::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                TD->getLocation()))
            return;

        templateDecl = TD;
    } else if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                ED->getLocation()))
            return;

        if (templateDecl) {
            std::string templateName = templateDecl->getNameAsString();
            std::string enumName = ED->getNameAsString();
            std::string mutation = "static_assert(" + templateName + "<" + enumName + ">::value, \"Expecting a complete type.\");\n/*mut36*/";
            Rewrite.InsertTextAfterToken(ED->getEndLoc(), mutation);
        }
    }
}

void MutatorFrontendAction_36::MutatorASTConsumer_36::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;

    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = decl().bind("TemplateDecl");
    DeclarationMatcher enumMatcher = enumDecl(unless(isDefinition())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.matchAST(Context);
}