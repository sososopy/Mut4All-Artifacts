//header file
#pragma once
#include "Mutator_base.h"

/**
 * insert_incomplete_type_reference_6
 */ 
class MutatorFrontendAction_6 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(6)

private:
    class MutatorASTConsumer_6 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_6(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Insert_Incomplete_Type_Reference_6.h"

// ========================================================================================================
#define MUT6_OUTPUT 1

void MutatorFrontendAction_6::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;
      
      std::string className = CL->getNameAsString();
      if (className.empty())
        return;

      auto content = stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      std::string insertion = "\n/*mut6*/" + className + " member;\n";
      if (content.find('{') != std::string::npos)
        content.insert(content.find('{') + 1, insertion);

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_6::MutatorASTConsumer_6::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}