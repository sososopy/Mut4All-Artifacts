//source file
#include "../include/Mutator_Insert_Incomplete_Template_Specialization_In_Anonymous_Namespace_In_Member_Function_363.h"

// ========================================================================================================
#define MUT363_OUTPUT 1

void MutatorFrontendAction_363::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        //Record template classes
        if (TD->getDescribedTemplate() != nullptr) {
            templateClasses.push_back(TD);
        }
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       TD->getLocation()))
            return;
        //Record type declarations
        typeDecls.push_back(TD);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;
        //Check if method has body
        if (!MT->hasBody())
            return;
        //Check if we have collected necessary components
        if (templateClasses.empty() || typeDecls.empty())
            return;
        //Get the source code text of target node
        auto bodyRange = MT->getBody()->getSourceRange();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
        //Select random template class and type
        size_t templateIdx = getrandom::getRandomIndex(templateClasses.size() - 1);
        size_t typeIdx = getrandom::getRandomIndex(typeDecls.size() - 1);
        const clang::CXXRecordDecl* templateClass = templateClasses[templateIdx];
        const clang::TypeDecl* typeDecl = typeDecls[typeIdx];
        //Generate concatenated identifier
        std::string baseName = typeDecl->getNameAsString();
        std::string concatenatedId = "std" + baseName;
        //Construct insertion text
        std::string insertText = "\nnamespace { struct : " + templateClass->getNameAsString() + "< " + typeDecl->getNameAsString() + ", char, " + concatenatedId + " > dummy_mut363 {}; }\n";
        insertText = "/*mut363*/" + insertText;
        //Find position to insert (after opening brace of function body)
        size_t openBracePos = bodyText.find('{');
        if (openBracePos != std::string::npos) {
            bodyText.insert(openBracePos + 1, insertText);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
    }
}
  
void MutatorFrontendAction_363::MutatorASTConsumer_363::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = cxxRecordDecl().bind("TemplateClass");
    DeclarationMatcher typeMatcher = typedefDecl().bind("TypeDecl");
    DeclarationMatcher methodMatcher = cxxMethodDecl(hasBody(compoundStmt())).bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}