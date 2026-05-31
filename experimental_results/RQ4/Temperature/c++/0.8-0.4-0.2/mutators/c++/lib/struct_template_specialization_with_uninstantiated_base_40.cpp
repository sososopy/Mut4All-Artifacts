//source file
#include "../include/Struct_Template_Specialization_With_Uninstantiated_Base_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *BT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseTemplate")) {
        if (!BT || !Result.Context->getSourceManager().isWrittenInMainFile(BT->getLocation()))
            return;
        if (!BT->isCompleteDefinition())
            return;
        baseTemplate = BT;
    } else if (auto *DT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedTemplate")) {
        if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(DT->getLocation()))
            return;
        if (!DT->isCompleteDefinition())
            return;
        derivedTemplate = DT;
    }

    if (baseTemplate && derivedTemplate) {
        // Get the source code text of target node
        std::string baseName = baseTemplate->getNameAsString();
        std::string derivedName = derivedTemplate->getNameAsString();

        // Perform mutation on the source code text by applying string replacement
        std::string specialization = "template<> struct " + derivedName + "<void> : public " + baseName + "<void> {};\n";
        std::string functionTemplate = "template<class T> void testFunction(" + derivedName + "<void> d) {\n"
                                       "    auto x = static_cast<const void*>(&d);\n"
                                       "}\n"
                                       "void test() {\n"
                                       "    testFunction(" + derivedName + "<void>());\n"
                                       "}\n";

        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(derivedTemplate->getEndLoc(), "\n/*mut40*/" + specialization + functionTemplate);

        // Reset pointers to avoid repeated mutations
        baseTemplate = nullptr;
        derivedTemplate = nullptr;
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto baseMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("BaseTemplate");
    auto derivedMatcher = cxxRecordDecl(isTemplateInstantiation(), has(cxxBaseSpecifier())).bind("DerivedTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}