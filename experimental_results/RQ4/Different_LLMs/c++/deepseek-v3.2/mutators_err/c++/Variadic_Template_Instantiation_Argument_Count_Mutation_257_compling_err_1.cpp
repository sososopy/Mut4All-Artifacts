//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Instantiation_Argument_Count_Mutation_257
 */ 
class MutatorFrontendAction_257 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(257)
private:
    class MutatorASTConsumer_257 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_257(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Variadic_Template_Instantiation_Argument_Count_Mutation_257.h"

// ========================================================================================================
#define MUT257_OUTPUT 1

void MutatorFrontendAction_257::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TI = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      
      // Get the template declaration
      const TemplateDecl *TD = TI->getTemplateName().getAsTemplateDecl();
      if (!TD) return;
      
      // Check if it's a variadic template
      const TemplateParameterList *TPL = TD->getTemplateParameters();
      if (!TPL || !TPL->containsUnexpandedParameterPack()) return;
      
      // Get the template arguments
      const TemplateArgumentList &Args = TI->getTemplateArgs();
      unsigned NumArgs = Args.size();
      if (NumArgs < 2) return;
      
      // Get source range of the template specialization
      SourceRange Range = TI->getSourceRange();
      if (Range.isInvalid()) return;
      
      // Get the original source text
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), Range);
      
      // Extract template name and arguments
      std::string TemplateName = TD->getNameAsString();
      std::vector<std::string> ArgumentStrings;
      
      // Build argument strings from existing types
      for (unsigned i = 0; i < NumArgs; ++i) {
        const TemplateArgument &Arg = Args[i];
        if (Arg.getKind() == TemplateArgument::Type) {
          QualType QT = Arg.getAsType();
          ArgumentStrings.push_back(QT.getAsString());
        } else {
          // For non-type arguments, skip mutation
          return;
        }
      }
      
      // Perform mutation: either increase or decrease argument count
      int MutationChoice = getrandom::getRandomIndex(1);
      std::string MutatedText;
      
      if (MutationChoice == 0 && NumArgs > 2) {
        // Decrease: remove last argument
        ArgumentStrings.pop_back();
      } else {
        // Increase: duplicate last argument
        ArgumentStrings.push_back(ArgumentStrings.back());
      }
      
      // Build mutated template instantiation
      MutatedText = TemplateName + "<";
      for (size_t i = 0; i < ArgumentStrings.size(); ++i) {
        MutatedText += ArgumentStrings[i];
        if (i < ArgumentStrings.size() - 1) MutatedText += ", ";
      }
      MutatedText += ">";
      
      // Add mutation marker
      MutatedText = "/*mut257*/" + MutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Range), MutatedText);
    }
}
  
void MutatorFrontendAction_257::MutatorASTConsumer_257::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}