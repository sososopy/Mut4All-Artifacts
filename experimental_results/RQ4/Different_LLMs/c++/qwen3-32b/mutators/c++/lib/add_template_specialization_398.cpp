//source file
#include "../include/Add_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *classTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("nonTypeClassTemplate")) {
        if (!classTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(classTemplate->getLocation()))
            return;

        auto primaryTemplate = classTemplate->getTemplatedDecl();
        if (!primaryTemplate || !primaryTemplate->isRecord())
            return;

        auto methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("nonTypeMethod");
        if (!methodDecl)
            return;

        std::string className = classTemplate->getNameAsString();
        std::string methodName = methodDecl->getNameAsString();

        std::string code = "template<> auto " + className + "<T>::" + methodName + "<5>() { return 5; }\n";
        code += "template struct " + className + "<int>;\n";

        auto loc = primaryTemplate->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, code);
    } else if (auto *classTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("staticDefaultClassTemplate")) {
        if (!classTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(classTemplate->getLocation()))
            return;

        auto primaryTemplate = classTemplate->getTemplatedDecl();
        if (!primaryTemplate || !primaryTemplate->isRecord())
            return;

        auto methodDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("staticDefaultMethod");
        if (!methodDecl)
            return;

        std::string className = classTemplate->getNameAsString();
        std::string methodName = methodDecl->getNameAsString();

        std::string code = "template<> void " + className + "<T>::" + methodName + "<int>() { }\n";
        code += "template struct " + className + "<char>;\n";

        auto loc = primaryTemplate->getEndLoc();
        Rewrite.InsertTextAfterToken(loc, code);
    }
}
  
void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = clang::ast_matchers::classTemplateDecl(
        clang::ast_matchers::has(clang::ast_matchers::templateDecl(
            clang::ast_matchers::isCXXMethod(),
            clang::ast_matchers::hasDescendant(clang::ast_matchers::nonTypeTemplateParmDecl())
        ).bind("nonTypeMethod"))
    ).bind("nonTypeClassTemplate");

    DeclarationMatcher matcher2 = clang::ast_matchers::classTemplateDecl(
        clang::ast_matchers::has(clang::ast_matchers::templateDecl(
            clang::ast_matchers::hasDeclaration(clang::ast_matchers::cxxMethodDecl(clang::ast_matchers::isStatic())),
            clang::ast_matchers::hasDescendant(clang::ast_matchers::nonTypeTemplateParmDecl(clang::ast_matchers::hasDefaultArgument()))
        ).bind("staticDefaultMethod"))
    ).bind("staticDefaultClassTemplate");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}