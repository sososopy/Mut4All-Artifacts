//source file
#include "../include/Insert_Module_Interface_Without_Implementation_81.h"

// ========================================================================================================
#define MUT81_OUTPUT 1

void MutatorFrontendAction_81::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (mutationApplied) return;
    
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("FirstGlobalDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;
        //Check if there's already a module declaration before this
        SourceManager &SM = *Result.SourceManager;
        SourceLocation StartLoc = VD->getBeginLoc();
        std::pair<FileID, unsigned> StartLocInfo = SM.getDecomposedLoc(StartLoc);
        const char *Buffer = SM.getBufferData(StartLocInfo.first).data();
        const char *StartPtr = Buffer + StartLocInfo.second;
        const char *FileStart = Buffer;
        
        //Scan backwards for module declarations
        bool hasModule = false;
        std::string contentBefore(FileStart, StartPtr);
        if (contentBefore.find("export module") != std::string::npos ||
            contentBefore.find("module;") != std::string::npos ||
            contentBefore.find("module ") != std::string::npos) {
            return;
        }
        
        //Get the source code text of target node location
        //Perform mutation on the source code text by applying string replacement
        std::string moduleDecl = "export module M;\n";
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(StartLoc, "/*mut81*/" + moduleDecl);
        mutationApplied = true;
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FirstGlobalDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (FD->isMain()) return; // Skip main function
        
        SourceManager &SM = *Result.SourceManager;
        SourceLocation StartLoc = FD->getBeginLoc();
        std::pair<FileID, unsigned> StartLocInfo = SM.getDecomposedLoc(StartLoc);
        const char *Buffer = SM.getBufferData(StartLocInfo.first).data();
        const char *StartPtr = Buffer + StartLocInfo.second;
        const char *FileStart = Buffer;
        
        //Scan for existing module declarations
        bool hasModule = false;
        std::string contentBefore(FileStart, StartPtr);
        if (contentBefore.find("export module") != std::string::npos ||
            contentBefore.find("module;") != std::string::npos ||
            contentBefore.find("module ") != std::string::npos) {
            return;
        }
        
        //Perform mutation
        std::string moduleDecl = "export module M;\n";
        Rewrite.InsertTextBefore(StartLoc, "/*mut81*/" + moduleDecl);
        mutationApplied = true;
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("FirstGlobalDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;
            
        SourceManager &SM = *Result.SourceManager;
        SourceLocation StartLoc = RD->getBeginLoc();
        std::pair<FileID, unsigned> StartLocInfo = SM.getDecomposedLoc(StartLoc);
        const char *Buffer = SM.getBufferData(StartLocInfo.first).data();
        const char *StartPtr = Buffer + StartLocInfo.second;
        const char *FileStart = Buffer;
        
        //Scan for existing module declarations
        std::string contentBefore(FileStart, StartPtr);
        if (contentBefore.find("export module") != std::string::npos ||
            contentBefore.find("module;") != std::string::npos ||
            contentBefore.find("module ") != std::string::npos) {
            return;
        }
        
        //Perform mutation
        std::string moduleDecl = "export module M;\n";
        Rewrite.InsertTextBefore(StartLoc, "/*mut81*/" + moduleDecl);
        mutationApplied = true;
    }
}
  
void MutatorFrontendAction_81::MutatorASTConsumer_81::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match the first global declaration that's not a module declaration
    DeclarationMatcher matcher = 
        anyOf(
            varDecl(isExpansionInMainFile(), unless(hasAncestor(decl(anyOf(
                namespaceDecl(),
                recordDecl(),
                functionDecl()
            ))))).bind("FirstGlobalDecl"),
            functionDecl(isExpansionInMainFile(), unless(isMain()), unless(hasAncestor(decl(anyOf(
                namespaceDecl(),
                recordDecl()
            ))))).bind("FirstGlobalDecl"),
            recordDecl(isExpansionInMainFile(), unless(hasAncestor(decl(anyOf(
                namespaceDecl(),
                recordDecl(),
                functionDecl()
            ))))).bind("FirstGlobalDecl")
        );
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}