//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Undeclared_Identifier_In_Template_Argument_List_254
 */ 
class MutatorFrontendAction_254 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(254)

private:
    class MutatorASTConsumer_254 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_254(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> declaredIdentifiers;
    };
};

//source file
#include "../include/Insert_Undeclared_Identifier_In_Template_Argument_List_254.h"

// ========================================================================================================
#define MUT254_OUTPUT 1

void MutatorFrontendAction_254::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TE = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TE->getTypeLoc().getBeginLoc()))
        return;
      
      //Collect declared identifiers from the AST context
      if (declaredIdentifiers.empty()) {
        auto &SM = *Result.SourceManager;
        auto &Context = *Result.Context;
        for (auto *D : Context.getTranslationUnitDecl()->decls()) {
          if (auto *ND = dyn_cast<NamedDecl>(D)) {
            if (SM.isWrittenInMainFile(ND->getLocation())) {
              declaredIdentifiers.insert(ND->getNameAsString());
            }
          }
        }
      }
      
      //Get the source code text of target node
      auto &SM = *Result.SourceManager;
      auto Loc = TE->getTypeLoc();
      auto Range = Loc.getSourceRange();
      std::string OriginalText = stringutils::rangetoStr(SM, Range);
      
      //Find template argument positions
      size_t OpenPos = OriginalText.find('<');
      size_t ClosePos = OriginalText.rfind('>');
      if (OpenPos == std::string::npos || ClosePos == std::string::npos || OpenPos >= ClosePos)
        return;
      
      std::string ArgList = OriginalText.substr(OpenPos + 1, ClosePos - OpenPos - 1);
      std::vector<std::string> Args;
      size_t Start = 0;
      size_t CommaPos;
      
      //Split arguments by commas, handling nested templates
      while (Start < ArgList.length()) {
        size_t NestCount = 0;
        size_t SearchPos = Start;
        while (SearchPos < ArgList.length()) {
          if (ArgList[SearchPos] == '<') NestCount++;
          else if (ArgList[SearchPos] == '>') NestCount--;
          else if (ArgList[SearchPos] == ',' && NestCount == 0) break;
          SearchPos++;
        }
        CommaPos = SearchPos;
        Args.push_back(ArgList.substr(Start, CommaPos - Start));
        Start = CommaPos + 1;
      }
      
      if (Args.empty()) return;
      
      //Select an argument to replace
      size_t ArgIndex = getrandom::getRandomIndex(Args.size() - 1);
      std::string SelectedArg = Args[ArgIndex];
      
      //Generate undeclared identifier
      std::string UndeclaredId;
      do {
        UndeclaredId = "UndeclaredType_" + std::to_string(getrandom::getRandomInt(1000));
      } while (declaredIdentifiers.find(UndeclaredId) != declaredIdentifiers.end());
      
      //Perform mutation on the source code text by applying string replacement
      Args[ArgIndex] = UndeclaredId;
      std::string NewArgList;
      for (size_t i = 0; i < Args.size(); ++i) {
        if (i > 0) NewArgList += ", ";
        NewArgList += Args[i];
      }
      std::string MutatedText = OriginalText.substr(0, OpenPos + 1) + NewArgList + OriginalText.substr(ClosePos);
      MutatedText = "/*mut254*/" + MutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Range), MutatedText);
    }
}
  
void MutatorFrontendAction_254::MutatorASTConsumer_254::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateSpecializationType().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}