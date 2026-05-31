//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inconsistent_Template_Specialization_214
 */ 
class MutatorFrontendAction_214 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(214)

private:
    class MutatorASTConsumer_214 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_214(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> specializedClasses;
    };
};

//source file
#include "../include/Mutator_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        std::string className = CTD->getNameAsString();
        if (specializedClasses.find(className) != specializedClasses.end())
            return;

        specializedClasses.insert(className);

        std::string specialization1 = "template<typename T>\nclass " + className + "<T*> {\n/* Specialized class definition */\n};\n";
        std::string specialization2 = "template<typename T>\nclass " + className + "<T**> {\n/* Conflicting specialization */\n};\n";

        SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, "\n/*mut214*/" + specialization1 + specialization2);
    }
}
  
void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}