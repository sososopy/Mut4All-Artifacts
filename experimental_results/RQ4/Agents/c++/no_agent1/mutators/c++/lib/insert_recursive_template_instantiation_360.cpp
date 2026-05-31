//source file
#include "../include/insert_recursive_template_instantiation_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        if (!RD->isCompleteDefinition() || RD->isLambda() || processedClasses.count(RD))
            return;

        processedClasses.insert(RD);

        std::string className = RD->getNameAsString();
        std::string templateCode = "template<int N> struct " + className + " {\n";
        templateCode += "  static const int value = " + className + "<N-1>::value + 1;\n";
        templateCode += "};\n";
        templateCode += "template<> struct " + className + "<0> {\n";
        templateCode += "  static const int value = 0;\n";
        templateCode += "};\n";

        Rewrite.InsertTextBefore(RD->getBeginLoc(), "/*mut360*/" + templateCode);
    }
}

void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isClass(), isTemplateInstantiation()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}