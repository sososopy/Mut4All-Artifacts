//source file
#include "../include/Insert_Inline_Namespace_With_Forward_Decl_390.h"

// ========================================================================================================
#define MUT390_OUTPUT 1

void MutatorFrontendAction_390::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("NSDecl")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;

        auto nsRange = NS->getSourceRange();
        auto nsText = stringutils::rangetoStr(*Result.SourceManager, nsRange);
        if (nsText.empty())
            return;

        size_t openBracePos = nsText.find('{');
        if (openBracePos == std::string::npos)
            return;

        std::string inlineNsName = "Mut_NS_390";
        std::string className = "Mut_Class_390";
        std::string insertText = "inline namespace " + inlineNsName + " {\nclass " + className + ";\n}";
        std::string newText = nsText.substr(0, openBracePos + 1) + "\n" + insertText + nsText.substr(openBracePos + 1);

        Rewrite.ReplaceText(nsRange, newText);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
            return;

        if (!CD->isCompleteDefinition())
            return;

        auto classRange = CD->getSourceRange();
        auto classText = stringutils::rangetoStr(*Result.SourceManager, classRange);
        if (classText.empty())
            return;

        std::string className = CD->getNameAsString();
        if (className.empty())
            return;

        std::string inlineNs = "inline namespace " + className + " {\n";
        std::string closing = "\n}";
        std::string newText = inlineNs + classText + closing;

        Rewrite.ReplaceText(classRange, newText);
    }
}

void MutatorFrontendAction_390::MutatorASTConsumer_390::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher nsMatcher = namespaceDecl().bind("NSDecl");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(nsMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}