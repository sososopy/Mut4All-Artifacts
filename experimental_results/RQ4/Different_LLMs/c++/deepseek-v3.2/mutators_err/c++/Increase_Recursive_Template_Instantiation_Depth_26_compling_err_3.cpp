//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increase_Recursive_Template_Instantiation_Depth_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Increase_Recursive_Template_Instantiation_Depth_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getTemplateName().getAsTemplateDecl()->getLocation()))
        return;
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto Loc = TE->getTemplateName().getAsTemplateDecl()->getLocation();
      if (Loc.isInvalid()) return;
      std::string OriginalText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(Loc, Loc));
      
      //Check if any template argument is an integer literal
      bool Modified = false;
      std::string MutatedText = OriginalText;
      const TemplateArgumentList *Args = TE->getArgs();
      
      for (unsigned i = 0; i < Args->size(); ++i) {
        const TemplateArgument &Arg = Args->get(i);
        if (Arg.getKind() == TemplateArgument::Integral) {
          llvm::APSInt OriginalValue = Arg.getAsIntegral();
          llvm::APSInt NewValue;
          if (OriginalValue == 0) {
            NewValue = llvm::APSInt::get(10); // Set zero to small positive
          } else {
            // Multiply by 10 to increase depth
            NewValue = OriginalValue * 10;
          }
          
          // Find the position of this argument in the source text
          // This is simplified; actual implementation needs to locate exact text
          std::string OldValStr = std::to_string(OriginalValue.getExtValue());
          std::string NewValStr = std::to_string(NewValue.getExtValue());
          
          size_t Pos = MutatedText.find(OldValStr);
          if (Pos != std::string::npos) {
            MutatedText.replace(Pos, OldValStr.length(), NewValStr);
            Modified = true;
            break; // Modify only one argument per instantiation
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      if (Modified) {
        MutatedText = "/*mut26*/" + MutatedText;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Loc, Loc), MutatedText);
      }
    }
}
  
void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}