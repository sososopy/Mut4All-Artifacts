//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Compound_Literal_With_Embed_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingFiles;
    };
};

//source file
#include "../include/Replace_Compound_Literal_With_Embed_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CompoundLiteralExpr>("CompoundLiteral")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto init = CL->getInitializer();
      if (!init) return;
      
      //Check if initializer is an InitListExpr (list of initializers)
      if (auto *ILE = dyn_cast<InitListExpr>(init)) {
        SourceRange initRange = ILE->getSourceRange();
        if (initRange.isInvalid()) return;
        
        //Collect existing files in the current directory or from includes
        SourceManager &SM = Rewrite.getSourceMgr();
        std::string currentFile = SM.getFilename(CL->getBeginLoc()).str();
        std::string currentDir = currentFile.substr(0, currentFile.find_last_of("/\\"));
        
        //Look for existing files: first check common headers already included
        std::vector<std::string> includedFiles;
        for (auto &File : SM.getLocalSLocEntryFileIDs()) {
          std::string fname = SM.getFilename(SM.getLocForStartOfFile(File)).str();
          if (fname.find(".h") != std::string::npos || fname.find(".inc") != std::string::npos) {
            existingFiles.push_back(fname);
          }
        }
        
        //If no headers found, create a dummy file in current directory
        std::string embedFile;
        if (existingFiles.empty()) {
          embedFile = currentDir + "/dummy_embed.bin";
          //In practice we would create this file, but for mutation we just use the path
        } else {
          //Pick a random existing file
          size_t idx = getrandom::getRandomIndex(existingFiles.size() - 1);
          embedFile = existingFiles[idx];
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string embedDirective = "#embed \"" + embedFile + "\"";
        
        //Replace the entire initializer content with the #embed directive
        //Replace from the opening brace to the closing brace
        SourceLocation braceStart = initRange.getBegin();
        SourceLocation braceEnd = initRange.getEnd();
        
        //Find the opening brace position
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, initRange);
        size_t openBracePos = originalText.find('{');
        size_t closeBracePos = originalText.rfind('}');
        
        if (openBracePos != std::string::npos && closeBracePos != std::string::npos) {
          //Calculate positions relative to braceStart
          unsigned openOffset = Result.SourceManager->getCharacterData(braceStart) - 
                                Result.SourceManager->getCharacterData(initRange.getBegin());
          unsigned closeOffset = Result.SourceManager->getCharacterData(braceEnd) - 
                                 Result.SourceManager->getCharacterData(initRange.getBegin());
          
          //Replace from '{' to '}' with new content
          SourceLocation replaceStart = braceStart.getLocWithOffset(openOffset);
          SourceLocation replaceEnd = braceStart.getLocWithOffset(closeOffset);
          
          std::string replacement = "{\n" + embedDirective + "\n}";
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(SourceRange(replaceStart, replaceEnd), replacement);
        }
      }
    }
}
  
void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = compoundLiteralExpr().bind("CompoundLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}