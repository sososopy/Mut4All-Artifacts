//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Range_With_Ternary_100
 */ 
class MutatorFrontendAction_100 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(100)

private:
    class MutatorASTConsumer_100 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_100(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> arrayDecls;
        std::vector<const clang::VarDecl*> boolDecls;
    };
};

//source file
#include "../include/Mutator_Replace_Range_With_Ternary_100.h"

// ========================================================================================================
#define MUT100_OUTPUT 1

void MutatorFrontendAction_100::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("arrayDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record array/container declarations
      if (VD->getType()->isArrayType() || VD->getType()->isRecordType()) {
        arrayDecls.push_back(VD);
      }
    }
    else if (auto *BD = Result.Nodes.getNodeAs<clang::VarDecl>("boolDecl")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;
      //Record boolean variable declarations
      if (BD->getType()->isBooleanType()) {
        boolDecls.push_back(BD);
      }
    }
    else if (auto *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forStmt")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      
      //Check if it's a range-based for loop
      if (!FS->getInit() && FS->getCondition() && FS->getInc()) {
        return; // Not a range-based for
      }
      
      //Get the range expression
      if (auto *RFS = dyn_cast<clang::CXXForRangeStmt>(FS)) {
        //Get the source code text of target node
        auto *RangeExpr = RFS->getRangeInit()->IgnoreImplicit();
        if (!RangeExpr) return;
        
        //Get range expression source range
        SourceRange RangeRange = RFS->getRangeInit()->getSourceRange();
        if (RangeRange.isInvalid()) return;
        
        //Prepare condition and containers for ternary
        std::string conditionExpr;
        std::string container1, container2;
        
        //Select boolean condition
        if (!boolDecls.empty()) {
          size_t idx = getrandom::getRandomIndex(boolDecls.size() - 1);
          conditionExpr = boolDecls[idx]->getNameAsString();
        } else {
          //No boolean variable found, create one at function start
          conditionExpr = "true";
          //Find enclosing function
          auto *Parent = Result.Context->getParents(*FS)[0].get<clang::Decl>();
          if (auto *FD = dyn_cast_or_null<clang::FunctionDecl>(Parent)) {
            if (FD->hasBody()) {
              SourceLocation InsertLoc = FD->getBody()->getBeginLoc().getLocWithOffset(1);
              Rewrite.InsertText(InsertLoc, "\nbool mut100_cond = true;\n");
              conditionExpr = "mut100_cond";
            }
          }
        }
        
        //Select containers
        if (!arrayDecls.empty()) {
          size_t idx1 = getrandom::getRandomIndex(arrayDecls.size() - 1);
          container1 = arrayDecls[idx1]->getNameAsString();
          
          if (arrayDecls.size() > 1) {
            size_t idx2;
            do {
              idx2 = getrandom::getRandomIndex(arrayDecls.size() - 1);
            } while (idx2 == idx1 && arrayDecls.size() > 1);
            container2 = arrayDecls[idx2]->getNameAsString();
          } else {
            container2 = container1; // Use same container if only one exists
          }
        } else {
          //No containers found, use the original range
          container1 = container2 = stringutils::rangetoStr(*(Result.SourceManager), RangeRange);
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string newRange = conditionExpr + " ? " + container1 + " : " + container2;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(RangeRange, newRange);
      }
    }
}
  
void MutatorFrontendAction_100::MutatorASTConsumer_100::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher arrayMatcher = varDecl(hasType(hasCanonicalType(anyOf(arrayType(), recordType())))).bind("arrayDecl");
    DeclarationMatcher boolMatcher = varDecl(hasType(booleanType())).bind("boolDecl");
    StatementMatcher forMatcher = forStmt().bind("forStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(arrayMatcher, &callback);
    matchFinder.addMatcher(boolMatcher, &callback);
    matchFinder.addMatcher(forMatcher, &callback);
    matchFinder.matchAST(Context);
}