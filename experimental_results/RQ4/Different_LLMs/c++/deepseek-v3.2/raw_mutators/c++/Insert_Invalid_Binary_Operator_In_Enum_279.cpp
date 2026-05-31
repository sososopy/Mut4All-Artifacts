//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Binary_Operator_In_Enum_279
 */ 
class MutatorFrontendAction_279 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(279)

private:
    class MutatorASTConsumer_279 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_279(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::EnumDecl*> visitedEnums;
    };
};

//source file
#include "../include/Insert_Invalid_Binary_Operator_In_Enum_279.h"

// ========================================================================================================
#define MUT279_OUTPUT 1

void MutatorFrontendAction_279::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Get the source code text of target node
      auto enumRange = ED->getSourceRange();
      std::string enumText = stringutils::rangetoStr(*(Result.SourceManager), enumRange);
      
      //Record the node information to be used in the mutation process
      visitedEnums.push_back(ED);
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedEnumText;
      
      //Check if enum has constants
      bool hasConstants = false;
      for (auto it = ED->enumerator_begin(); it != ED->enumerator_end(); ++it) {
          hasConstants = true;
          break;
      }
      
      std::string enumName = ED->getNameAsString();
      std::string newConstantName;
      if (enumName.empty()) {
          newConstantName = "mut_279";
      } else {
          newConstantName = enumName + "_mut";
      }
      
      std::string binaryOperator = getrandom::getRandomBinaryOperator();
      
      if (hasConstants) {
          //Use existing enumerators
          std::vector<std::string> enumeratorNames;
          for (auto it = ED->enumerator_begin(); it != ED->enumerator_end(); ++it) {
              enumeratorNames.push_back(it->getNameAsString());
          }
          
          if (enumeratorNames.size() >= 2) {
              size_t idx1 = getrandom::getRandomIndex(enumeratorNames.size() - 1);
              size_t idx2 = getrandom::getRandomIndex(enumeratorNames.size() - 1);
              std::string lhs = enumeratorNames[idx1];
              std::string rhs = enumeratorNames[idx2];
              
              //Find the position of the enum body
              size_t bodyStart = enumText.find('{');
              size_t bodyEnd = enumText.find('}');
              if (bodyStart != std::string::npos && bodyEnd != std::string::npos) {
                  std::string bodyContent = enumText.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
                  //Insert binary operation before the closing brace
                  mutatedEnumText = enumText.substr(0, bodyEnd);
                  mutatedEnumText += lhs + " " + binaryOperator + " " + rhs + ";";
                  mutatedEnumText += enumText.substr(bodyEnd);
              } else {
                  mutatedEnumText = enumText;
              }
          } else {
              //Only one enumerator, use it twice
              std::string enumerator = enumeratorNames[0];
              size_t bodyStart = enumText.find('{');
              size_t bodyEnd = enumText.find('}');
              if (bodyStart != std::string::npos && bodyEnd != std::string::npos) {
                  std::string bodyContent = enumText.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
                  mutatedEnumText = enumText.substr(0, bodyEnd);
                  mutatedEnumText += enumerator + " " + binaryOperator + " " + enumerator + ";";
                  mutatedEnumText += enumText.substr(bodyEnd);
              } else {
                  mutatedEnumText = enumText;
              }
          }
      } else {
          //No constants, introduce new constant with binary operation
          size_t bodyStart = enumText.find('{');
          size_t bodyEnd = enumText.find('}');
          if (bodyStart != std::string::npos && bodyEnd != std::string::npos) {
              mutatedEnumText = enumText.substr(0, bodyStart + 1);
              mutatedEnumText += newConstantName + " " + binaryOperator + " " + newConstantName;
              mutatedEnumText += enumText.substr(bodyEnd);
          } else {
              //If no braces found, insert them
              mutatedEnumText = enumText + " { " + newConstantName + " " + binaryOperator + " " + newConstantName + " }";
          }
      }
      
      mutatedEnumText = "/*mut279*/" + mutatedEnumText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(enumRange), mutatedEnumText);
    }
}
  
void MutatorFrontendAction_279::MutatorASTConsumer_279::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}