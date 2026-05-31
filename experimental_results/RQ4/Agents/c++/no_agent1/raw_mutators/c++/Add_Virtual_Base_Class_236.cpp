//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Virtual_Base_Class_236
 */ 
class MutatorFrontendAction_236 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(236)

private:
    class MutatorASTConsumer_236 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_236(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::CXXRecordDecl *> visitedClasses;
    };
};

//source file
#include "../include/add_virtual_base_class_236.h"

// ========================================================================================================
#define MUT236_OUTPUT 1

void MutatorFrontendAction_236::Callback::run(const MatchFinder::MatchResult &Result) {
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl")) {
        if (!recordDecl)
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc()))
            return;
        if (!recordDecl->isCompleteDefinition() || recordDecl->isLambda())
            return;

        if (visitedClasses.find(recordDecl) == visitedClasses.end()) {
            visitedClasses.insert(recordDecl);

            std::string className = recordDecl->getNameAsString();
            if (className.empty())
                return;

            std::string virtualBaseClass = "VirtualBase_" + className;
            std::string baseClassDefinition = "class " + virtualBaseClass + " {};\n";
            std::string derivedClassModification = " : virtual public " + virtualBaseClass;

            // Insert the virtual base class definition before the current class
            Rewrite.InsertTextBefore(recordDecl->getBeginLoc(), baseClassDefinition);

            // Modify the current class to inherit from the virtual base class
            Rewrite.InsertTextAfterToken(recordDecl->getBeginLoc(), derivedClassModification);
        }
    }
}

void MutatorFrontendAction_236::MutatorASTConsumer_236::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}