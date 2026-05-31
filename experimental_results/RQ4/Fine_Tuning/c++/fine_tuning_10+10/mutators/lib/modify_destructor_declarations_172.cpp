//source file
#include "../include/Modify_Destructor_Declarations_172.h"

// ========================================================================================================
#define MUT172_OUTPUT 1

void MutatorFrontendAction_172::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
            return;
        if (!CL->isCompleteDefinition())
            return;
        if (visitedClasses.find(CL) != visitedClasses.end())
            return;
        
        visitedClasses.insert(CL);
        
        auto *Destructor = CL->getDestructor();
        std::string className = CL->getNameAsString();
        std::string dtorDeclaration;
        
        if (!Destructor) {
            dtorDeclaration = "virtual ~" + className + "() = delete;";
        } else {
            if (!Destructor->isVirtual()) {
                dtorDeclaration = "virtual ~" + className + "() = delete;";
            } else {
                dtorDeclaration = "~" + className + "() = default;";
            }
        }
        
        dtorDeclaration = "\n/*mut172*/" + dtorDeclaration + "\n";
        Rewrite.InsertTextAfterToken(CL->getEndLoc(), dtorDeclaration);
    }
}

void MutatorFrontendAction_172::MutatorASTConsumer_172::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}