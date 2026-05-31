//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Member_Function_With_Dependent_Base_Call_274
 */ 
class MutatorFrontendAction_274 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(274)

private:
    class MutatorASTConsumer_274 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_274(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/replace_template_member_function_with_dependent_base_call_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto rtype = MT->getReturnType().getAsString();
      auto name = MT->getNameAsString();
      auto param = MT->getQualifiedNameAsString();
      auto replace = "(this->*static_cast<" + rtype + " (DependentBase::*)" +
                     param + ">(&DependentBase::" + name + "))()";
      llvm::outs() << replace << '\n';
      if (content.rfind('}') != string::npos)
        content.replace(content.rfind('}'), 1, "/*mut274*/" + replace + "}");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasParent(cxxRecordDecl(
                                                   hasAnyTemplateArgument(
                                                       refersToType(hasDescendant(
                                                           dependentNameType()))))))
                                     .bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}