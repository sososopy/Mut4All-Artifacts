//source file
#include "../include/Mutator_Swap_Base_Class_Initializer_Order_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("CtorWithBaseInits")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;
      
      // Get the class being constructed
      const CXXRecordDecl *ClassDecl = Ctor->getParent();
      if (!ClassDecl || ClassDecl->getNumBases() < 2)
        return;
      
      // Get the constructor's initializer list
      const CXXCtorInitializer *FirstBaseInit = nullptr;
      const CXXCtorInitializer *SecondBaseInit = nullptr;
      int BaseInitCount = 0;
      
      for (const auto *Init : Ctor->inits()) {
        if (Init->isBaseInitializer()) {
          if (BaseInitCount == 0) {
            FirstBaseInit = Init;
          } else if (BaseInitCount == 1) {
            SecondBaseInit = Init;
          }
          BaseInitCount++;
        }
      }
      
      // Need at least two base class initializers to swap
      if (BaseInitCount < 2)
        return;
      
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      
      // Get the full source range of the constructor including the initializer list
      SourceRange CtorRange = Ctor->getSourceRange();
      if (!CtorRange.isValid())
        return;
      
      // Get the source text of the constructor
      std::string CtorText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(CtorRange), SM, LangOpts).str();
      
      // Find the initializer list section
      size_t InitListStart = CtorText.find(':');
      if (InitListStart == std::string::npos)
        return;
      
      // Find the positions of the two base initializers in the text
      // We need to locate them by their source ranges
      SourceRange FirstBaseRange = FirstBaseInit->getSourceRange();
      SourceRange SecondBaseRange = SecondBaseInit->getSourceRange();
      
      if (!FirstBaseRange.isValid() || !SecondBaseRange.isValid())
        return;
      
      // Get the text of each base initializer
      std::string FirstBaseText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(FirstBaseRange), SM, LangOpts).str();
      std::string SecondBaseText = Lexer::getSourceText(
          CharSourceRange::getTokenRange(SecondBaseRange), SM, LangOpts).str();
      
      // Get the full initializer list text
      size_t BodyStart = CtorText.find('{', InitListStart);
      if (BodyStart == std::string::npos)
        return;
      
      std::string InitListText = CtorText.substr(InitListStart, BodyStart - InitListStart);
      
      // Find the positions of the base initializers within the init list text
      size_t FirstPos = InitListText.find(FirstBaseText);
      size_t SecondPos = InitListText.find(SecondBaseText);
      
      if (FirstPos == std::string::npos || SecondPos == std::string::npos)
        return;
      
      // Create the swapped init list text
      std::string SwappedInitListText = InitListText;
      
      // We need to be careful about commas between initializers
      // Replace the first occurrence with the second base initializer
      SwappedInitListText.replace(FirstPos, FirstBaseText.length(), SecondBaseText);
      
      // Now we need to find the position of the second base initializer in the modified text
      // Since we replaced text, the position may have changed
      size_t NewSecondPos = SwappedInitListText.find(SecondBaseText, FirstPos + SecondBaseText.length());
      if (NewSecondPos != std::string::npos) {
        // Replace this occurrence with the first base initializer
        SwappedInitListText.replace(NewSecondPos, SecondBaseText.length(), FirstBaseText);
      } else {
        // Fallback: if we can't find it, reconstruct the entire init list
        // Build a list of all initializers
        std::vector<std::string> Inits;
        std::string CurrentInit;
        bool InBaseInit = false;
        
        // Simple parsing to split initializers by commas
        for (size_t i = 1; i < InitListText.length(); ++i) {
          char c = InitListText[i];
          if (c == '(') InBaseInit = true;
          else if (c == ')') InBaseInit = false;
          
          if (!InBaseInit && (c == ',' || i == InitListText.length() - 1)) {
            if (i == InitListText.length() - 1) CurrentInit += c;
            if (!CurrentInit.empty()) {
              Inits.push_back(CurrentInit);
              CurrentInit.clear();
            }
          } else {
            CurrentInit += c;
          }
        }
        
        // Identify which indices are base initializers
        std::vector<int> BaseInitIndices;
        for (size_t i = 0; i < Inits.size(); ++i) {
          // Check if this init matches either base initializer text
          if (Inits[i].find(FirstBaseText) != std::string::npos ||
              Inits[i].find(SecondBaseText) != std::string::npos) {
            BaseInitIndices.push_back(i);
          }
        }
        
        // Swap the base initializers if we found exactly two
        if (BaseInitIndices.size() == 2) {
          std::swap(Inits[BaseInitIndices[0]], Inits[BaseInitIndices[1]]);
        }
        
        // Reconstruct the init list
        SwappedInitListText = ":";
        for (size_t i = 0; i < Inits.size(); ++i) {
          SwappedInitListText += Inits[i];
          if (i != Inits.size() - 1) {
            SwappedInitListText += ",";
          }
        }
      }
      
      //Replace the original AST node with the mutated one
      // Replace the entire constructor text with the swapped version
      std::string SwappedCtorText = CtorText;
      SwappedCtorText.replace(InitListStart, InitListText.length(), SwappedInitListText);
      
      // Add mutation marker
      size_t InsertPos = SwappedCtorText.find('{');
      if (InsertPos != std::string::npos) {
        SwappedCtorText.insert(InsertPos, "/*mut17*/");
      }
      
      Rewrite.ReplaceText(CtorRange, SwappedCtorText);
    }
}
  
void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(
      hasAnyConstructorInitializer(
        isBaseInitializer()
      ),
      unless(isImplicit())
    ).bind("CtorWithBaseInits");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}