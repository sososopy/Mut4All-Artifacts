//source file
#include "../include/insert_virtual_base_class_314.h"

// ========================================================================================================
#define MUT314_OUTPUT 1

void MutatorFrontendAction_314::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("BaseClasses")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        
        baseClasses.insert(CL->getNameAsString());
    } 
    else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedClasses")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(DL->getLocation()))
            return;
        if (DL->isCompleteDefinition() == false)
            return;

        std::string insertion;
        for (const auto &base : baseClasses) {
            insertion += "virtual " + base + ", ";
        }
        if (!insertion.empty()) {
            insertion = ": " + insertion.substr(0, insertion.size() - 2); // Remove the last comma and space
        }

        std::string derivedClassText = "class Mut_314 " + insertion + " { /*mut314*/ };\n";
        Rewrite.InsertTextAfterToken(DL->getEndLoc(), derivedClassText);
    }
}

void MutatorFrontendAction_314::MutatorASTConsumer_314::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher baseMatcher = cxxRecordDecl().bind("BaseClasses");
    DeclarationMatcher derivedMatcher = cxxRecordDecl().bind("DerivedClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.addMatcher(derivedMatcher, &callback);
    matchFinder.matchAST(Context);
}