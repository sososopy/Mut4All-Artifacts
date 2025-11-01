//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Inline_Variable_Template_38
 */ 
class MutatorFrontendAction_38 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(38)

private:
    class MutatorASTConsumer_38 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_38(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_inline_variable_template_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CRD->getLocation()))
        return;
      if (!CRD->isCompleteDefinition())
        return;
      
      //Get the source code text of target node
      std::string insertion = "template<typename T> inline constexpr int value = 42;\n";
      insertion = "/*mut38*/" + insertion;
      
      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = CRD->getBeginLoc().getLocWithOffset(1);
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, insertion);
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}