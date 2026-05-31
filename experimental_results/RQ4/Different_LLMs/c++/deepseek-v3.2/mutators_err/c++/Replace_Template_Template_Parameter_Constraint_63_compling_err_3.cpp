//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Parameter_Constraint_63
 */ 
class MutatorFrontendAction_63 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(63)

private:
    class MutatorASTConsumer_63 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_63(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Template_Parameter_Constraint_63.h"

// ========================================================================================================
#define MUT63_OUTPUT 1

void MutatorFrontendAction_63::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTemplateParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      
      if (!TP->isTemplateParameterPack()) {
        //Get the source code text of target node
        auto sourceRange = TP->getSourceRange();
        auto originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
        
        //Collect available types from the context
        if (availableTypes.empty()) {
          availableTypes = {"int", "char", "bool", "float", "double"};
          //Add more built-in types
          availableTypes.push_back("short");
          availableTypes.push_back("long");
          availableTypes.push_back("unsigned");
          availableTypes.push_back("void*");
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText;
        int choice = getrandom::getRandomIndex(2); // 0: replace with type, 1: remove constraint, 2: keep original
        
        if (choice == 0 && !availableTypes.empty()) {
          //Replace concept constraint with a concrete type
          size_t typeIndex = getrandom::getRandomIndex(availableTypes.size() - 1);
          std::string newType = availableTypes[typeIndex];
          
          //Find the concept constraint part
          size_t templateStart = originalText.find("template <");
          if (templateStart != std::string::npos) {
            size_t constraintStart = originalText.find('<', templateStart + 10);
            if (constraintStart != std::string::npos) {
              size_t constraintEnd = originalText.find('>', constraintStart);
              if (constraintEnd != std::string::npos) {
                //Replace the constraint
                mutatedText = originalText.substr(0, constraintStart + 1) + 
                             newType + 
                             originalText.substr(constraintEnd);
              }
            }
          }
        } else if (choice == 1) {
          //Remove the constraint entirely
          size_t templateStart = originalText.find("template <");
          if (templateStart != std::string::npos) {
            size_t constraintStart = originalText.find('<', templateStart + 10);
            if (constraintStart != std::string::npos) {
              size_t constraintEnd = originalText.find('>', constraintStart);
              if (constraintEnd != std::string::npos) {
                //Replace with typename
                mutatedText = originalText.substr(0, constraintStart + 1) + 
                             "typename" + 
                             originalText.substr(constraintEnd);
              }
            }
          }
        }
        
        //Replace the original AST node with the mutated one
        if (!mutatedText.empty() && mutatedText != originalText) {
          mutatedText = "/*mut63*/" + mutatedText;
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_63::MutatorASTConsumer_63::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(
        hasAncestor(
            templateDecl(
                has(
                    templateTypeParmDecl().bind("InnerTemplateParm")
                )
            )
        )
    ).bind("TemplateTemplateParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}