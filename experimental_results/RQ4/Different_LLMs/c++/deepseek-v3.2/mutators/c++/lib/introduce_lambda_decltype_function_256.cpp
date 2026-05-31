//source file
#include "../include/Mutator_Introduce_Lambda_Decltype_Function_256.h"

// ========================================================================================================
#define MUT256_OUTPUT 1

void MutatorFrontendAction_256::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        //Record available function declarations for decltype
        if (FD->isThisDeclarationADefinition()) {
            availableFunctions.push_back(FD);
        }
    }
    else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
        //Filter nodes in header files
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;
        
        //Get the template function definition
        auto *TemplatedFunction = FT->getTemplatedDecl();
        if (!TemplatedFunction || !TemplatedFunction->hasBody())
            return;
        
        //Check if we have any available functions for decltype
        std::string functionName;
        if (availableFunctions.empty()) {
            //Add abort declaration at top of file if not present
            if (!abortDeclAdded) {
                SourceLocation startLoc = Rewrite.getSourceMgr().getLocForStartOfFile(Rewrite.getSourceMgr().getMainFileID());
                Rewrite.InsertText(startLoc, "void abort();\n");
                abortDeclAdded = true;
            }
            functionName = "abort";
        } else {
            //Pick a random available function
            int index = getrandom::getRandomIndex(availableFunctions.size() - 1);
            functionName = availableFunctions[index]->getNameAsString();
        }
        
        //Get the source code text of target node
        auto bodyRange = TemplatedFunction->getBody()->getSourceRange();
        auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), bodyRange);
        
        //Perform mutation on the source code text by applying string replacement
        std::string lambdaInsertion = "\n/*mut256*/[](int)->decltype(" + functionName + "){};";
        //Insert lambda at beginning of function body
        size_t insertPos = bodyText.find("{") + 1;
        bodyText.insert(insertPos, lambdaInsertion);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyText);
    }
}
  
void MutatorFrontendAction_256::MutatorASTConsumer_256::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.matchAST(Context);
}