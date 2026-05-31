//source file
#include "../include/insert_virtual_base_class_232.h"

// ========================================================================================================
#define MUT232_OUTPUT 1

void MutatorFrontendAction_232::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        //Filter nodes in header files
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;
        
        //Record the base class name
        baseClasses.insert(Base->getNameAsString());
    }
    else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        //Filter nodes in header files
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;

        //Get the source code text of target node
        std::string derivedCode = stringutils::rangetoStr(*(Result.SourceManager), Derived->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        if (!baseClasses.empty()) {
            std::string virtualBase = "virtual " + *baseClasses.begin();
            size_t pos = derivedCode.find(":");
            if (pos != std::string::npos) {
                derivedCode.insert(pos + 1, " " + virtualBase + ",");
            } else {
                derivedCode.insert(derivedCode.find("{"), ": " + virtualBase);
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Derived->getSourceRange()), derivedCode);
        }
    }
}
  
void MutatorFrontendAction_232::MutatorASTConsumer_232::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition(), hasAnyBase(cxxBaseSpecifier())).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}