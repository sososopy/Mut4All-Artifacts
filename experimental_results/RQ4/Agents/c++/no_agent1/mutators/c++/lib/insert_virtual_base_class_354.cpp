//source file
#include "../include/insert_virtual_base_class_354.h"

// ========================================================================================================
#define MUT354_OUTPUT 1

void MutatorFrontendAction_354::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BaseClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!BaseClass || !Result.Context->getSourceManager().isWrittenInMainFile(BaseClass->getLocation()))
            return;
        if (!BaseClass->isCompleteDefinition())
            return;
        baseClasses.insert(BaseClass->getNameAsString());
    } else if (auto *DerivedClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DerivedClass || !Result.Context->getSourceManager().isWrittenInMainFile(DerivedClass->getLocation()))
            return;
        if (!DerivedClass->isCompleteDefinition())
            return;
        
        std::string insertionText = "\n/*mut354*/";
        for (const auto &baseName : baseClasses) {
            insertionText += "class " + baseName + ";\n";
        }
        
        std::string derivedName = DerivedClass->getNameAsString();
        insertionText += "class Mut354_" + derivedName + " : virtual public " + *baseClasses.begin() + " {};\n";
        
        Rewrite.InsertTextAfterToken(DerivedClass->getEndLoc(), insertionText);
    }
}

void MutatorFrontendAction_354::MutatorASTConsumer_354::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDerivedFrom(hasName("BaseClass"))).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}