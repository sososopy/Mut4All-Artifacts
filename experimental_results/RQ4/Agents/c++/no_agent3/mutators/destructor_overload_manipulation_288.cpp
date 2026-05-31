//header file
#pragma once
#include "Mutator_base.h"

/**
 * Destructor_Overload_Manipulation_288
 */ 
class MutatorFrontendAction_288 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(288)

private:
    class MutatorASTConsumer_288 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_288(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::set<const CXXRecordDecl*> processedClasses;
    };
};

//source file
#include "../include/destructor_overload_manipulation_288.h"

// ========================================================================================================
#define MUT288_OUTPUT 1

void MutatorFrontendAction_288::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
            return;
        if (!CL->isCompleteDefinition() || processedClasses.count(CL))
            return;

        auto *Destructor = CL->getDestructor();
        if (Destructor && Destructor->isUserProvided()) {
            // Remove user-defined destructor
            SourceRange DtorRange = Destructor->getSourceRange();
            Rewrite.RemoveText(DtorRange);
        }

        bool hasBaseClass = false;
        for (auto &Base : CL->bases()) {
            if (const CXXRecordDecl *BaseDecl = Base.getType()->getAsCXXRecordDecl()) {
                if (BaseDecl->hasDefinition() && BaseDecl->getDestructor() && !BaseDecl->getDestructor()->isVirtual()) {
                    // Make existing base class destructor virtual
                    SourceLocation Loc = BaseDecl->getDestructor()->getLocation();
                    Rewrite.InsertText(Loc, "virtual ", true, true);
                }
                hasBaseClass = true;
            }
        }

        if (!hasBaseClass) {
            // Introduce a new base class with a virtual destructor
            std::string baseClassDecl = "class Base { public: virtual ~Base() {} }; ";
            Rewrite.InsertTextBefore(CL->getBeginLoc(), baseClassDecl);

            // Make the current class inherit from the new base class
            std::string inheritance = " : public Base";
            Rewrite.InsertTextAfterToken(CL->getLocation(), inheritance);
        }

        processedClasses.insert(CL);
    }
}
  
void MutatorFrontendAction_288::MutatorASTConsumer_288::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxDestructorDecl(isUserProvided()))).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}