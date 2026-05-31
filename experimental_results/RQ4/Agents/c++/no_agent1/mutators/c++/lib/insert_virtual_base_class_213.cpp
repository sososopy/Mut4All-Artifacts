//source file
#include "../include/insert_virtual_base_class_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Base = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!Base || !Result.Context->getSourceManager().isWrittenInMainFile(Base->getLocation()))
            return;
        if (!Base->isCompleteDefinition() || Base->isLambda())
            return;
        baseClasses.insert(Base->getNameAsString());
    } else if (auto *Derived = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!Derived || !Result.Context->getSourceManager().isWrittenInMainFile(Derived->getLocation()))
            return;
        if (!Derived->isCompleteDefinition() || Derived->isLambda())
            return;
        
        std::string insertionText;
        for (const auto &base : baseClasses) {
            insertionText += "virtual public " + base + ", ";
        }
        if (!insertionText.empty()) {
            insertionText.pop_back(); // Remove last comma
            insertionText.pop_back(); // Remove last space
            insertionText = " : " + insertionText;
        }
        
        if (!insertionText.empty()) {
            SourceLocation insertLoc = Derived->getLocation();
            Rewrite.InsertTextAfterToken(insertLoc, insertionText);
        }
    }
}

void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}