//source file
#include "../include/insert_recursive_template_instantiation_376.h"

// ========================================================================================================
#define MUT376_OUTPUT 1

void MutatorFrontendAction_376::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                    CTD->getLocation()))
            return;
        if (!CTD->isThisDeclarationADefinition())
            return;

        templates.push_back(CTD);

        // Get the source code text of target node
        auto templateName = CTD->getNameAsString();
        std::string recursiveTemplate = "template<int N> struct " + templateName + " {\n";
        recursiveTemplate += "    " + templateName + "<N-1> next;\n";
        recursiveTemplate += "};\n";
        recursiveTemplate += "template<> struct " + templateName + "<0> {};\n";

        // Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTD->getSourceRange()), recursiveTemplate);
    }
}

void MutatorFrontendAction_376::MutatorASTConsumer_376::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}