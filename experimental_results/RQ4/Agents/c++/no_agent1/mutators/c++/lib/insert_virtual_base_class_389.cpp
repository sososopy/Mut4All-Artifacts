//source file
#include "../include/insert_virtual_base_class_389.h"

// ========================================================================================================
#define MUT389_OUTPUT 1

void MutatorFrontendAction_389::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        
        processedClasses.insert(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClass")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (!DL->isCompleteDefinition())
            return;
        
        for (auto *Base : processedClasses) {
            if (DL->isDerivedFrom(Base))
                return;
        }

        std::string baseClassName = (*processedClasses.begin())->getNameAsString();
        std::string derivedClassName = DL->getNameAsString();
        std::string mutationText = "class " + derivedClassName + " : virtual public " + baseClassName + " {};\n";
        mutationText = "\n/*mut389*/" + mutationText;
        
        Rewrite.InsertTextAfter(DL->getEndLoc(), mutationText);
    }
}

void MutatorFrontendAction_389::MutatorASTConsumer_389::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl(isDefinition()).bind("BaseClass");
    DeclarationMatcher derivedMatcher = cxxRecordDecl(isDefinition()).bind("DerivedClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}