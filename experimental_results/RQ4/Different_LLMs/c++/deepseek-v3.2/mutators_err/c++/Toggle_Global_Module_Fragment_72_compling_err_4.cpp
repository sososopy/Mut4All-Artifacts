//header file
#pragma once
#include "Mutator_base.h"

/**
 * Toggle_Global_Module_Fragment_72
 */ 
class MutatorFrontendAction_72 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(72)

private:
    class MutatorASTConsumer_72 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_72(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Toggle_Global_Module_Fragment_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::Decl>("ModuleDecl")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      
      SourceManager &SM = *Result.SourceManager;
      LangOptions LangOpts = Result.Context->getLangOpts();
      
      // Get the module interface unit's main file
      FileID FID = SM.getMainFileID();
      if (FID.isInvalid()) return;
      
      // Check if this is a module interface unit (has export module)
      bool isModuleInterface = false;
      SourceLocation ModuleLoc;
      std::string ModuleName;
      
      // Find the export module declaration
      if (auto *ModDecl = dyn_cast<clang::ModuleDecl>(MD)) {
        for (auto *Child : ModDecl->decls()) {
          if (auto *ED = dyn_cast<clang::ExportDecl>(Child)) {
            for (auto *SubChild : ED->decls()) {
              if (auto *InnerModDecl = dyn_cast<clang::ModuleDecl>(SubChild)) {
                isModuleInterface = true;
                ModuleLoc = ED->getBeginLoc();
                break;
              }
            }
            if (isModuleInterface) break;
          }
        }
      }
      
      if (!isModuleInterface || ModuleLoc.isInvalid()) return;
      
      // Check if there's a global module fragment before the export module
      bool hasGMF = false;
      SourceLocation GMFStart;
      SourceLocation GMFEnd;
      std::string GMFContent;
      
      // Look for module; directive before the export module
      SourceLocation FileStart = SM.getLocForStartOfFile(FID);
      SourceLocation BeforeModule = ModuleLoc.getLocWithOffset(-1);
      
      if (FileStart.isValid() && BeforeModule.isValid()) {
        const char *BufferStart = SM.getCharacterData(FileStart);
        const char *BufferEnd = SM.getCharacterData(BeforeModule);
        
        // Search for "module;" in the file before the export module
        std::string Prefix(BufferStart, BufferEnd - BufferStart);
        size_t pos = Prefix.find("module;");
        if (pos != std::string::npos) {
          hasGMF = true;
          GMFStart = FileStart.getLocWithOffset(pos);
          
          // Find the end of the global module fragment (just before export module)
          // Look for newline after module; to get the full GMF content
          size_t line_end = Prefix.find('\n', pos);
          if (line_end != std::string::npos) {
            // Get everything from module; to just before export module
            GMFEnd = FileStart.getLocWithOffset(line_end);
            GMFContent = Prefix.substr(pos, line_end - pos);
            
            // Check if there are more declarations in GMF (multiple lines)
            while (line_end < Prefix.length()) {
              size_t next_line = Prefix.find('\n', line_end + 1);
              if (next_line == std::string::npos) break;
              
              std::string line = Prefix.substr(line_end + 1, next_line - line_end - 1);
              // Skip empty lines and preprocessor directives
              if (line.empty() || line[0] == '#') {
                line_end = next_line;
                GMFEnd = FileStart.getLocWithOffset(next_line);
                continue;
              }
              
              // Check if this line starts export module
              size_t export_pos = line.find("export module");
              if (export_pos != std::string::npos) {
                break;
              }
              
              // This line is part of GMF
              GMFContent += "\n" + line;
              line_end = next_line;
              GMFEnd = FileStart.getLocWithOffset(next_line);
            }
          }
        }
      }
      
      // Perform mutation: toggle GMF presence
      if (hasGMF) {
        // Remove GMF: delete from module; to just before export module
        if (GMFStart.isValid() && GMFEnd.isValid()) {
          CharSourceRange GMFRange = CharSourceRange::getCharRange(GMFStart, GMFEnd);
          Rewrite.RemoveText(GMFRange);
        }
      } else {
        // Insert GMF: add "module;" with a simple declaration before export module
        std::string NewGMF = "module;\nint hidden() { return 2; }\n";
        Rewrite.InsertText(ModuleLoc, NewGMF, true, true);
      }
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decl().bind("ModuleDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}