//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Braces_To_Array_Initializer_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Add_Braces_To_Array_Initializer_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("ArrayVarDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto *init = MT->getInit();
      if (!init) return;
      auto *initExpr = dyn_cast<InitListExpr>(init);
      if (!initExpr) return;
      //Check if array type
      auto *type = MT->getType().getTypePtrOrNull();
      if (!type || !type->isArrayType()) return;
      //Check element type is struct/aggregate
      auto *elemType = type->getArrayElementTypeNoTypeQual();
      if (!elemType->isStructureType() && !elemType->isAggregateType()) return;
      
      //Get initializer list
      unsigned numInits = initExpr->getNumInits();
      if (numInits == 0) return;
      
      //Check for at least one element with cast/expression
      bool hasComplexInit = false;
      for (unsigned i = 0; i < numInits; ++i) {
        auto *initElem = initExpr->getInit(i);
        if (isa<CastExpr>(initElem) || 
            (isa<StringLiteral>(initElem) && elemType->isPointerType()) ||
            !isa<ConstantExpr>(initElem)) {
          hasComplexInit = true;
          break;
        }
      }
      if (!hasComplexInit) return;
      
      //Check if element initializers already have braces
      bool alreadyBraced = false;
      for (unsigned i = 0; i < numInits; ++i) {
        auto *initElem = initExpr->getInit(i);
        if (isa<InitListExpr>(initElem)) {
          alreadyBraced = true;
          break;
        }
      }
      if (alreadyBraced) return;
      
      //Get source range
      SourceRange range = initExpr->getSourceRange();
      if (range.isInvalid()) return;
      
      //Get original text
      std::string original = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Perform mutation: add braces around each element
      std::string mutated = "{";
      for (unsigned i = 0; i < numInits; ++i) {
        auto *initElem = initExpr->getInit(i);
        SourceRange elemRange = initElem->getSourceRange();
        std::string elemText = stringutils::rangetoStr(*(Result.SourceManager), elemRange);
        
        //Check if struct has multiple fields
        if (elemType->isStructureType()) {
          auto *record = elemType->getAsRecordDecl();
          if (record && record->getNumFields() > 1) {
            //Expand to initialize all fields
            mutated += "{ " + elemText;
            for (unsigned j = 1; j < record->getNumFields(); ++j) {
              mutated += ", 0";
            }
            mutated += " }";
          } else {
            mutated += "{ " + elemText + " }";
          }
        } else {
          mutated += "{ " + elemText + " }";
        }
        if (i < numInits - 1) mutated += ", ";
      }
      mutated += "}";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(range, mutated);
    }
}
  
void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr())).bind("ArrayVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}