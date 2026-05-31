//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Empty_Struct_Union_Initializer_263
 */ 
class MutatorFrontendAction_263 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(263)

private:
    class MutatorASTConsumer_263 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_263(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingUnionNames;
        std::vector<std::string> existingVarNames;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT263_OUTPUT 1

void MutatorFrontendAction_263::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Check if initializer is empty braces
      const Expr* Init = VD->getInit();
      if (!Init) return;
      
      // Check if it's a compound initializer with empty list
      if (const InitListExpr* ILE = dyn_cast<InitListExpr>(Init)) {
        if (ILE->getNumInits() == 0) {
          // Get the source code text of target node
          auto initRange = VD->getInit()->getSourceRange();
          std::string initText = stringutils::rangetoStr(*Result.SourceManager, initRange);
          
          // Check if variable is union or struct type
          QualType QT = VD->getType();
          const Type* T = QT.getTypePtr();
          bool isUnion = false;
          bool isStruct = false;
          
          if (const RecordType* RT = T->getAs<RecordType>()) {
            RecordDecl* RD = RT->getDecl();
            if (RD->isUnion()) isUnion = true;
            else if (RD->isStruct()) isStruct = true;
          }
          
          if (!isUnion && !isStruct) return;
          
          // Prepare union name 'f'
          std::string unionName = "f";
          bool unionExists = false;
          for (const auto& name : existingUnionNames) {
            if (name == unionName) {
              unionExists = true;
              break;
            }
          }
          
          // Prepare variable name 't'
          std::string varName = "t";
          bool varExists = false;
          for (const auto& name : existingVarNames) {
            if (name == varName) {
              varExists = true;
              break;
            }
          }
          
          // Perform mutation on the source code text by applying string replacement
          std::string mutatedText;
          if (unionExists) {
            mutatedText = "{ struct { typedef union " + unionName + "[" + varName + "]; } }";
          } else {
            mutatedText = "{ struct { typedef union " + unionName + "[" + varName + "]; } }";
          }
          
          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(initRange, mutatedText);
          
          // If union 'f' doesn't exist, insert declaration before this variable
          if (!unionExists) {
            SourceLocation InsertLoc = VD->getBeginLoc();
            std::string unionDecl = "\nunion " + unionName + " {};\n";
            Rewrite.InsertTextBefore(InsertLoc, unionDecl);
            existingUnionNames.push_back(unionName);
          }
          
          // If variable 't' doesn't exist, insert declaration before this variable
          if (!varExists) {
            SourceLocation InsertLoc = VD->getBeginLoc();
            std::string varDecl = "\nint " + varName + ";\n";
            Rewrite.InsertTextBefore(InsertLoc, varDecl);
            existingVarNames.push_back(varName);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_263::MutatorASTConsumer_263::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr(hasNumElements(0)))).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}