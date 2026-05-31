//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_StringView_Arg_With_Literal_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_AST_CONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check if the function being called has a std::string_view parameter
      const FunctionDecl *FD = CE->getDirectCallee();
      if (!FD) return;
      
      // Check arguments
      unsigned numArgs = CE->getNumArgs();
      if (numArgs == 0) return;
      
      // Find arguments that are of type std::string_view
      for (unsigned i = 0; i < numArgs; ++i) {
        const Expr *Arg = CE->getArg(i);
        if (!Arg) continue;
        
        QualType ArgType = Arg->getType();
        std::string ArgTypeStr = ArgType.getAsString();
        if (ArgTypeStr.find("std::basic_string_view") != std::string::npos ||
            ArgTypeStr.find("std::string_view") != std::string::npos) {
          continue;
        }
        
        // Check if the corresponding parameter is std::string_view
        if (i < FD->getNumParams()) {
          const ParmVarDecl *Param = FD->getParamDecl(i);
          QualType ParamType = Param->getType();
          std::string ParamTypeStr = ParamType.getAsString();
          
          if (ParamTypeStr.find("std::basic_string_view") != std::string::npos ||
              ParamTypeStr.find("std::string_view") != std::string::npos) {
            
            // Get the source range of the argument
            SourceRange ArgRange = Arg->getSourceRange();
            if (ArgRange.isInvalid()) continue;
            
            // Perform mutation: replace with string literal
            std::string replacement = "\"mutated_literal\"";
            
            // Replace the original AST node with the mutated one
            Rewrite.ReplaceText(ArgRange, replacement);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr().bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}