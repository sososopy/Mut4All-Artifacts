//source file
#include "../include/insert_recursive_templates_440.h"

// ========================================================================================================
#define MUT440_OUTPUT 1

void MutatorFrontendAction_440::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;
        
        auto className = RD->getNameAsString();
        if (className.empty())
            return;

        templateClasses.push_back(RD);

        // Perform mutation on the source code text by applying string replacement
        std::string templateDef = "template<int N> struct RecursiveTemplate_" + className + " {\n";
        templateDef += "    static const int value = N * RecursiveTemplate_" + className + "<N-1>::value;\n";
        templateDef += "};\n";
        templateDef += "template<> struct RecursiveTemplate_" + className + "<0> {\n";
        templateDef += "    static const int value = 1;\n";
        templateDef += "};\n";
        templateDef = "/*mut440*/" + templateDef;

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), "\n" + templateDef);
    }
}

void MutatorFrontendAction_440::MutatorASTConsumer_440::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isClass()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}