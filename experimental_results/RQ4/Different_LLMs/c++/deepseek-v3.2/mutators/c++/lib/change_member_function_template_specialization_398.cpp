//source file
#include "../include/Change_Member_Function_Template_Specialization_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Specialization")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (!FD->isFunctionTemplateSpecialization() || !FD->getInstantiatedFromMemberFunction())
            return;
        
        const clang::DeclContext* DC = FD->getParent();
        const clang::CXXRecordDecl* RD = clang::dyn_cast<clang::CXXRecordDecl>(DC);
        if (!RD || !RD->isCompleteDefinition())
            return;
        
        SourceLocation ClassEnd = RD->getEndLoc();
        SourceLocation SpecLoc = FD->getBeginLoc();
        if (!Result.SourceManager->isBeforeInTranslationUnit(SpecLoc, ClassEnd))
            return;
        
        specializations.push_back(FD);
    }
}

void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    // Implementation logic for handling translation unit
}