//source file
#include "../include/Mutator_Concatenate_Base_Class_Names_181.h"

// ========================================================================================================
#define MUT181_OUTPUT 1

void MutatorFrontendAction_181::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;
        if ((!RD->isStruct() && !RD->isClass()) || RD->isLambda())
            return;
        if (!RD->isCompleteDefinition())
            return;
        //Record available type names for potential concatenation
        availableTypes.push_back(RD);
    }
    else if (auto *BD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("baseDecl")) {
        //Filter nodes in header files
        if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       BD->getLocation()))
            return;
        if ((!BD->isStruct() && !BD->isClass()) || BD->isLambda())
            return;
        if (!BD->isCompleteDefinition())
            return;
        
        //Get the base class specifiers
        auto bases = BD->bases();
        if (bases.empty())
            return;
        
        //Get the source code text of target node
        SourceManager &SM = *Result.SourceManager;
        LangOptions LangOpts;
        SourceLocation startLoc = BD->getBeginLoc();
        SourceLocation endLoc = BD->getEndLoc();
        
        //Find the colon location
        SourceLocation colonLoc;
        for (auto it = SM.getCharacterData(startLoc); 
             it < SM.getCharacterData(endLoc); ++it) {
            if (*it == ':') {
                colonLoc = startLoc.getLocWithOffset(it - SM.getCharacterData(startLoc));
                break;
            }
        }
        if (colonLoc.isInvalid())
            return;
        
        //Find the base class list range
        SourceLocation baseStart = colonLoc.getLocWithOffset(1);
        SourceLocation baseEnd = colonLoc;
        bool foundBrace = false;
        for (auto it = SM.getCharacterData(baseStart); 
             it < SM.getCharacterData(endLoc); ++it) {
            if (*it == '{') {
                baseEnd = baseStart.getLocWithOffset(it - SM.getCharacterData(baseStart));
                foundBrace = true;
                break;
            }
        }
        if (!foundBrace)
            return;
        
        //Extract base class list text
        CharSourceRange baseRange = CharSourceRange::getCharRange(baseStart, baseEnd);
        std::string baseText = Lexer::getSourceText(baseRange, SM, LangOpts).str();
        
        //Trim whitespace
        baseText.erase(0, baseText.find_first_not_of(" \t\n\r"));
        baseText.erase(baseText.find_last_not_of(" \t\n\r") + 1);
        
        if (baseText.empty())
            return;
        
        //Parse base class names
        std::vector<std::string> baseNames;
        std::string currentName;
        bool inTemplate = false;
        int templateDepth = 0;
        
        for (char c : baseText) {
            if (c == '<') {
                inTemplate = true;
                templateDepth++;
                currentName += c;
            }
            else if (c == '>') {
                templateDepth--;
                if (templateDepth == 0) inTemplate = false;
                currentName += c;
            }
            else if (c == ',' && !inTemplate) {
                if (!currentName.empty()) {
                    //Trim access specifiers
                    size_t pos = currentName.find_last_of(" \t");
                    if (pos != std::string::npos) {
                        std::string lastWord = currentName.substr(pos + 1);
                        if (lastWord == "public" || lastWord == "protected" || lastWord == "private") {
                            currentName = currentName.substr(0, pos);
                        }
                    }
                    //Trim whitespace
                    currentName.erase(0, currentName.find_first_not_of(" \t"));
                    currentName.erase(currentName.find_last_not_of(" \t") + 1);
                    if (!currentName.empty()) {
                        baseNames.push_back(currentName);
                    }
                }
                currentName.clear();
            }
            else {
                currentName += c;
            }
        }
        
        //Add the last base name
        if (!currentName.empty()) {
            //Trim access specifiers
            size_t pos = currentName.find_last_of(" \t");
            if (pos != std::string::npos) {
                std::string lastWord = currentName.substr(pos + 1);
                if (lastWord == "public" || lastWord == "protected" || lastWord == "private") {
                    currentName = currentName.substr(0, pos);
                }
            }
            //Trim whitespace
            currentName.erase(0, currentName.find_first_not_of(" \t"));
            currentName.erase(currentName.find_last_not_of(" \t") + 1);
            if (!currentName.empty()) {
                baseNames.push_back(currentName);
            }
        }
        
        if (baseNames.empty())
            return;
        
        //Perform mutation: concatenate base class names
        std::string mutatedText;
        if (baseNames.size() > 1) {
            //Concatenate first two base names
            mutatedText = baseNames[0] + baseNames[1];
            //Add remaining bases
            for (size_t i = 2; i < baseNames.size(); ++i) {
                mutatedText += ", " + baseNames[i];
            }
            //Add trailing comma if there were multiple bases originally
            mutatedText += ",";
        }
        else {
            //Only one base class, concatenate with another type from available types
            if (!availableTypes.empty()) {
                size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
                std::string otherType = availableTypes[idx]->getNameAsString();
                if (!otherType.empty()) {
                    mutatedText = baseNames[0] + otherType;
                }
                else {
                    mutatedText = baseNames[0] + "UnknownType";
                }
            }
            else {
                mutatedText = baseNames[0] + "UnknownType";
            }
        }
        
        //Preserve access specifiers if present in original text
        std::string originalFirstBase = baseNames[0];
        size_t accessPos = baseText.find(originalFirstBase);
        if (accessPos > 0) {
            std::string prefix = baseText.substr(0, accessPos);
            //Check if prefix ends with an access specifier
            size_t lastSpace = prefix.find_last_of(" \t");
            if (lastSpace != std::string::npos) {
                std::string lastWord = prefix.substr(lastSpace + 1);
                if (lastWord == "public" || lastWord == "protected" || lastWord == "private") {
                    mutatedText = lastWord + " " + mutatedText;
                }
            }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(baseRange, " " + mutatedText + " ");
    }
}
  
void MutatorFrontendAction_181::MutatorASTConsumer_181::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("recordDecl");
    DeclarationMatcher baseMatcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("baseDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(baseMatcher, &callback);
    matchFinder.matchAST(Context);
}