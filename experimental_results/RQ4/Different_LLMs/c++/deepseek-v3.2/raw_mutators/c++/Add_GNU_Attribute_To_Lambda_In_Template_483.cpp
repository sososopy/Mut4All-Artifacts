//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_GNU_Attribute_To_Lambda_In_Template_483
 */ 
class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(483)

private:
    class MutatorASTConsumer_483 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_483(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> attribute_list = {"always_inline", "noinline", "deprecated"};
    };
};

//source file
#include "../include/Mutator_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      
      // Check if lambda is in templated context
      auto *Parent = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunc");
      if (!Parent) {
        Parent = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass");
      }
      if (!Parent) {
        return;
      }
      
      //Get the source code text of target node
      auto LambdaRange = L->getSourceRange();
      auto LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      // Find lambda introducer position
      size_t introducer_pos = LambdaText.find('[');
      if (introducer_pos == std::string::npos) {
        return;
      }
      
      // Find position after lambda parameters
      size_t param_end = LambdaText.find(']');
      if (param_end == std::string::npos) {
        return;
      }
      param_end++; // Move past ']'
      
      // Check for existing attribute
      size_t attr_pos = LambdaText.find("__attribute__");
      if (attr_pos != std::string::npos && attr_pos < LambdaText.find('{')) {
        // Lambda already has an attribute, skip mutation
        return;
      }
      
      //Choose random attribute
      int attr_index = getrandom::getRandomIndex(attribute_list.size() - 1);
      std::string attribute = "__attribute__((" + attribute_list[attr_index] + "))";
      
      //Perform mutation on the source code text by applying string replacement
      LambdaText.insert(param_end, " " + attribute);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), LambdaText);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(
      hasAncestor(
        anyOf(
          functionDecl(isTemplateInstantiation()).bind("TemplateFunc"),
          cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass")
        )
      )
    ).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}