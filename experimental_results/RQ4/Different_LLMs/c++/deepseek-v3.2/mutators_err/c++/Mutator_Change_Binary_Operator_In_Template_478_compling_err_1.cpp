//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Change_Binary_Operator_In_Template_478
 */ 
class MutatorFrontendAction_478 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(478)

private:
    class MutatorASTConsumer_478 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_478(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::vector<std::string>> operatorCategories;
    };
};

//source file
#include "../include/Mutator_Change_Binary_Operator_In_Template_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getLocation()))
        return;

      // Check if the binary operator is inside a template
      auto *Parent = Result.Nodes.getNodeAs<clang::Decl>("TemplateDecl");
      if (!Parent) return;

      // Check if at least one operand has a user-defined type
      auto *LHS = BO->getLHS()->IgnoreImpCasts();
      auto *RHS = BO->getRHS()->IgnoreImpCasts();
      bool hasUDT = false;
      if (LHS->getType()->isRecordType() || RHS->getType()->isRecordType()) {
        hasUDT = true;
      }
      if (!hasUDT) return;

      // Determine operator category
      std::string currentOp = BO->getOpcodeStr().str();
      std::string category;
      std::vector<std::string> alternatives;

      // Initialize operator categories
      operatorCategories["arithmetic"] = {"+", "-", "*", "/", "%"};
      operatorCategories["relational"] = {"==", "!=", "<", "<=", ">", ">="};
      operatorCategories["logical"] = {"&&", "||"};
      operatorCategories["bitwise"] = {"|", "&", "^", "<<", ">>"};
      operatorCategories["assignment"] = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="};

      // Find category
      for (const auto &cat : operatorCategories) {
        if (std::find(cat.second.begin(), cat.second.end(), currentOp) != cat.second.end()) {
          category = cat.first;
          alternatives = cat.second;
          break;
        }
      }
      if (category.empty()) return;

      // Remove current operator from alternatives
      alternatives.erase(std::remove(alternatives.begin(), alternatives.end(), currentOp), alternatives.end());
      if (alternatives.empty()) return;

      // Randomly select an alternative operator
      int idx = getrandom::getRandomIndex(alternatives.size() - 1);
      std::string newOp = alternatives[idx];

      //Get the source code text of target node
      auto opRange = BO->getOperatorLoc();
      SourceRange fullRange = BO->getSourceRange();
      auto fullText = stringutils::rangetoStr(*(Result.SourceManager), fullRange);

      //Find the operator position in the full text
      std::string opStr = BO->getOpcodeStr().str();
      size_t opPos = fullText.find(opStr);
      if (opPos == std::string::npos) return;

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = fullText;
      mutatedText.replace(opPos, opStr.length(), newOp);

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(fullRange), mutatedText);
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = binaryOperator(hasAncestor(decl(anyOf(
        classTemplateDecl(),
        functionTemplateDecl(),
        cxxRecordDecl(isTemplateInstantiation())
    )))).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}