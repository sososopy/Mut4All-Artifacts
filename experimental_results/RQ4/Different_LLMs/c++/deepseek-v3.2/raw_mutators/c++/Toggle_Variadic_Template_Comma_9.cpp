//header file
#pragma once
#include "Mutator_base.h"

/**
 * Toggle_Variadic_Template_Comma_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Toggle_Variadic_Template_Comma_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("VariadicTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto SourceRange = MT->getSourceRange();
      if (!SourceRange.isValid()) return;
      std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      
      //Check if this is a variadic template specialization
      const TemplateArgumentList &Args = MT->getTemplateArgs();
      if (Args.size() == 0) return;
      
      //Look for the variadic pack at the end
      bool HasVariadicPack = false;
      for (unsigned i = 0; i < Args.size(); ++i) {
        if (Args[i].isPackExpansion()) {
          HasVariadicPack = true;
          break;
        }
      }
      if (!HasVariadicPack) return;
      
      //Find the template argument list in the source code
      SourceLocation TemplateStart = MT->getLocation();
      SourceLocation TemplateEnd = SourceRange.getEnd();
      
      //Look for the angle brackets
      std::string SourceStr = OriginalText;
      size_t OpenAngle = SourceStr.find('<');
      if (OpenAngle == std::string::npos) return;
      size_t CloseAngle = SourceStr.rfind('>');
      if (CloseAngle == std::string::npos) return;
      
      //Extract the template arguments
      std::string ArgsStr = SourceStr.substr(OpenAngle + 1, CloseAngle - OpenAngle - 1);
      
      //Find the last comma before the variadic pack
      size_t LastCommaPos = std::string::npos;
      size_t VariadicPos = ArgsStr.find("...");
      if (VariadicPos == std::string::npos) return;
      
      //Check for comma immediately before the variadic pack
      bool HasCommaBeforeVariadic = false;
      if (VariadicPos > 0) {
        //Look backwards for the last non-whitespace character before "..."
        size_t CheckPos = VariadicPos - 1;
        while (CheckPos > 0 && std::isspace(ArgsStr[CheckPos])) {
          --CheckPos;
        }
        if (CheckPos < ArgsStr.length() && ArgsStr[CheckPos] == ',') {
          HasCommaBeforeVariadic = true;
          LastCommaPos = CheckPos;
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedArgsStr;
      if (HasCommaBeforeVariadic) {
        //Remove the comma before the variadic pack
        MutatedArgsStr = ArgsStr.substr(0, LastCommaPos);
        //Add the rest, skipping the comma
        size_t AfterComma = LastCommaPos + 1;
        while (AfterComma < ArgsStr.length() && std::isspace(ArgsStr[AfterComma])) {
          ++AfterComma;
        }
        MutatedArgsStr += ArgsStr.substr(AfterComma);
      } else {
        //Insert a comma before the variadic pack
        //Find the start of the last argument before "..."
        size_t LastArgStart = VariadicPos;
        while (LastArgStart > 0 && !std::isspace(ArgsStr[LastArgStart - 1]) && 
               ArgsStr[LastArgStart - 1] != ',' && ArgsStr[LastArgStart - 1] != '<') {
          --LastArgStart;
        }
        
        MutatedArgsStr = ArgsStr.substr(0, LastArgStart);
        //Add the last argument
        std::string LastArg = ArgsStr.substr(LastArgStart, VariadicPos - LastArgStart);
        MutatedArgsStr += LastArg + ", ";
        //Add the variadic pack and anything after it
        MutatedArgsStr += ArgsStr.substr(VariadicPos);
      }
      
      //Construct the mutated text
      std::string MutatedText = SourceStr.substr(0, OpenAngle + 1) + 
                               MutatedArgsStr + 
                               SourceStr.substr(CloseAngle);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(
        hasTemplateArgument(0, isPackExpansion())
    ).bind("VariadicTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}