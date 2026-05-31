//source file
#include "../include/Inline_Namespace_Template_Specialization_Modifier_415.h"

// ========================================================================================================
#define MUT415_OUTPUT 1

void MutatorFrontendAction_415::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("InlineNamespace")) {
        if (!NS || !NS->isInline() || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        inlineNamespace = NS;
    } else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDecl")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        templateDecl = FTD;
    }

    if (inlineNamespace && templateDecl) {
        // Get the source code text of target node
        auto templateName = templateDecl->getNameAsString();

        // Perform mutation on the source code text by applying string replacement
        std::string specializationText = "template <> " + templateName + "<int> " + templateName + "(int);\n";
        specializationText += "template <> " + templateName + "<float> " + templateName + "(float);\n";

        // Insert the specialization outside the inline namespace
        SourceLocation insertLoc = inlineNamespace->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "/*mut415*/\n" + specializationText);

        // Reset the pointers to avoid repeated insertions
        inlineNamespace = nullptr;
        templateDecl = nullptr;
    }
}
  
void MutatorFrontendAction_415::MutatorASTConsumer_415::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher inlineNamespaceMatcher = namespaceDecl(isInline()).bind("InlineNamespace");
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inlineNamespaceMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}